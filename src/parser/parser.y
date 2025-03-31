
%language "c++"
%define parse.error verbose
%define api.value.type {FrogTok}

%parse-param { FrogLexer& lexer }
%code requires { 
  #include <optional>
  #include <memory>
  #include <fstream>
  #include "ast.hpp"
  #include "froglexer.hpp"
  #include "tokens.hpp"
  extern std::ostream* diagnostic_stream;
}

%code {
  extern std::shared_ptr<ast::Node> ast_root;
  #define yylex lexer.yylex
}

%{

%}

%token <token> NUMBER
%token <token> IDENTIFIER
%token <token> TYPE_ID

%token <token> TRUE
%token <token> FALSE

%token <token> ARROW

%token <token> PLUS
%token <token> MINUS
%token <token> STAR
%token <token> SLASH
%token <token> PERCENT

%token <token> LESS
%token <token> GREATER
%token <token> LESSEQ
%token <token> GREATEREQ
%token <token> EQUALITY
%token <token> INEQUALITY

%token <token> LOGICAL_AND
%token <token> LOGICAL_OR
%token <token> NEGATION

%token <token> ASSIGNMENT

%token <token> FN
%token <token> RET

%token <token> LPAREN
%token <token> RPAREN

%token <token> LBRACKET
%token <token> RBRACKET

%token <token> LBRACE
%token <token> RBRACE
%token <token> SEMICOLON
%token <token> COMMA
%token <token> COLON

%token <token> IF
%token <token> ELSE

%token <token> WHILE

%left LOGICAL_OR
%left LOGICAL_AND
%left LESS GREATER LESSEQ GREATEREQ EQUALITY INEQUALITY
%left PLUS MINUS
%left STAR SLASH PERCENT

%right UMINUS
%right NEGATION

%type <node>
  expression program global_obj function
  function_declaration block arglist
  statement statements declaration
  args call_args call_arglist
  function_declaration_globject

// %type <node> rvalue lvalue

%%
program: /* */ {
    ast_root = std::make_shared<ast::ProgramNode>();
    $$ = ast_root;
    *diagnostic_stream<<"Parsing finished successfully\n";
  }
  | program global_obj {
    auto prog_node = dynamic_pointer_cast<ast::ProgramNode>($1);
    auto globject = dynamic_pointer_cast<ast::GlobjectNode>($2);
    prog_node->add_obj(globject);
    $$ = $1;
  }
  ;

global_obj: function {
    $$ = $1;
  }
  | function_declaration_globject {
    $$ = $1;
  }
  ;

function_declaration_globject: function_declaration SEMICOLON {
  auto decl = dynamic_pointer_cast<ast::FunctionDeclaration>($1);
  $$ = std::make_shared<ast::FunctionDeclarationGlobject>(decl);
}

function: function_declaration block {
    auto decl = dynamic_pointer_cast<ast::FunctionDeclaration>($1);
    auto body = dynamic_pointer_cast<ast::Block>($2);
    $$ = std::make_shared<ast::FunctionGlobject>(decl, body);
  }
  ;

function_declaration: FN IDENTIFIER arglist ARROW TYPE_ID {
    std::optional<llvm::Type*> type_ref = 
      CompilerContext::Types->get_type($5->metadata);
    if(!type_ref) {
      yy::parser::error("Undeclared type: \"" + $5->metadata + "\"");
      YYERROR;
    }
    $$ = std::make_shared<ast::FunctionDeclaration>(
      $2->metadata, 
      dynamic_pointer_cast<ast::FunctionArglist>($3),
      *type_ref);
  }
  | FN IDENTIFIER arglist {
    $$ = std::make_shared<ast::FunctionDeclaration>(
      $2->metadata, 
      dynamic_pointer_cast<ast::FunctionArglist>($3),
      llvm::Type::getVoidTy(*CompilerContext::TheContext));
  }
  ;

block: LBRACE statements RBRACE {
    auto statements = dynamic_pointer_cast<ast::Statements>($2);
    $$ = std::make_shared<ast::Block>(statements);
  }
  ;

statements: /* empty */ { 
    $$ = std::make_shared<ast::Statements>(); 
  }
  | statements statement {
    auto statements = dynamic_pointer_cast<ast::Statements>($1);
    auto statement = dynamic_pointer_cast<ast::StatementNode>($2); 
    statements->add_statement(statement);
    $$ = $1;
  }
  ;

statement: expression SEMICOLON {
    $$ = std::make_shared<ast::ExpressionStatement>(
      dynamic_pointer_cast<ast::ExprNode>($1));
  }
  | declaration SEMICOLON {
    $$ = std::make_shared<ast::DeclarationStatement>(
      dynamic_pointer_cast<ast::DeclarationNode>($1));
  }
  | declaration ASSIGNMENT expression SEMICOLON {
    auto decl = dynamic_pointer_cast<ast::DeclarationNode>($1);
    auto expr = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = std::make_shared<ast::DeclarationAssignmentStatement>(decl, expr);
  }
  | RET expression SEMICOLON {
    auto expr = dynamic_pointer_cast<ast::ExprNode>($2);
    $$ = std::make_shared<ast::ReturnStatement>(expr);
  }
  | RET SEMICOLON {
    $$ = std::make_shared<ast::ReturnStatement>();
  }
  | block {
    $$ = $1;
  } 
  | IF LPAREN expression RPAREN statement {
    auto condition = dynamic_pointer_cast<ast::ExprNode>($3);
    auto body = dynamic_pointer_cast<ast::StatementNode>($5);
    $$ = std::make_shared<ast::IfStatement>(condition, body);
  }
  | IF LPAREN expression RPAREN block ELSE block {
    auto condition = dynamic_pointer_cast<ast::ExprNode>($3);
    auto if_body = dynamic_pointer_cast<ast::StatementNode>($5);
    auto else_body = dynamic_pointer_cast<ast::StatementNode>($7);
    $$ = std::make_shared<ast::IfStatement>(condition, if_body, else_body);
  }
  ;

declaration: IDENTIFIER COLON TYPE_ID {
    std::optional<llvm::Type*> type_ref =
      CompilerContext::Types->get_type($3->metadata);
    if(!type_ref) {
      yy::parser::error("Undeclared type: \"" + $3->metadata + "\"");
      YYERROR;
    }
    $$ = std::make_shared<ast::DeclarationNode>(*type_ref, $1->metadata);
  }
  ;


call_arglist: LPAREN call_args RPAREN {
    $$ = $2;
  }

call_args: /* empty */ {
    $$ = std::make_shared<ast::FunctionCallArglist>();
  }
  | expression {
    auto call_args = std::make_shared<ast::FunctionCallArglist>();
    auto expr = dynamic_pointer_cast<ast::ExprNode>($1);
    call_args->add_arg(expr);
    $$ = call_args;
  }
  | call_args COMMA expression {
    auto call_args = dynamic_pointer_cast<ast::FunctionCallArglist>($1);
    auto expr = dynamic_pointer_cast<ast::ExprNode>($3);
    call_args->add_arg(expr);
    $$ = call_args;
  }
  ;

expression: NUMBER {
    //TODO: solve the type issue
    $$ = std::make_shared<ast::IntegerConstant>(32, $1->metadata);
  }
  | TRUE {
    $$ = std::make_shared<ast::IntegerConstant>(1, "1");
  }
  | FALSE {
    $$ = std::make_shared<ast::IntegerConstant>(1, "0");
  }
  | IDENTIFIER {
    $$ = std::make_shared<ast::VariableIdentifier>($1->metadata);
  }
  | LBRACKET expression RBRACKET {
    $$ = std::make_shared<ast::IversonExpr>(
      dynamic_pointer_cast<ast::ExprNode>($2));
  }
  | MINUS expression %prec UMINUS {
    $$ = ast::UnaryOperator::create(
      "-",
      dynamic_pointer_cast<ast::ExprNode>($2));
  }
  | NEGATION expression {
    $$ = ast::UnaryOperator::create(
      "~",
      dynamic_pointer_cast<ast::ExprNode>($2));
  }
  | IDENTIFIER call_arglist {
    auto call_arglist = dynamic_pointer_cast<ast::FunctionCallArglist>($2);
    $$ = std::make_shared<ast::FunctionCallExpr>($1->metadata, call_arglist);
  }
  | LPAREN expression RPAREN {$$ = $2;}
  | expression PLUS expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create("+", lhs, rhs);
  }
  | expression MINUS expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create("-", lhs, rhs);
  }
  | expression STAR expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create("*", lhs, rhs);
  }
  | expression SLASH expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create("/", lhs, rhs);
  }
  | expression PERCENT expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create("%", lhs, rhs);
  }
  | expression LESS expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create("<", lhs, rhs);
  }
  | expression GREATER expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create(">", lhs, rhs);
  }
  | expression LESSEQ expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create("<=", lhs, rhs);
  }
  | expression GREATEREQ expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create(">=", lhs, rhs);
}
  | expression EQUALITY expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create("==", lhs, rhs);
  }
  | expression INEQUALITY expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create("!=", lhs, rhs);
  }
  | expression LOGICAL_AND expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create("&&", lhs, rhs);
  }
  | expression LOGICAL_OR expression {
    auto lhs = dynamic_pointer_cast<ast::ExprNode>($1);
    auto rhs = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = ast::BinaryOperator::create("||", lhs, rhs);
  }
  ;

args: /* empty */ {
    $$ = std::make_shared<ast::FunctionArglist>();
  }
  | declaration {
    auto args = std::make_shared<ast::FunctionArglist>();
    auto arg = dynamic_pointer_cast<ast::DeclarationNode>($1);
    args->add_arg(arg);
    $$ = args;
  }
  | args COMMA declaration {
    auto args = dynamic_pointer_cast<ast::FunctionArglist>($1);
    auto arg = dynamic_pointer_cast<ast::DeclarationNode>($3);
    args->add_arg(arg);
    $$ = args;
  }
  ;

arglist: LPAREN args RPAREN {
    $$ = $2;
  }
  ;
%%

void yy::parser::error(const std::string &message) {
  std::cerr << "Error: " << message << std::endl;
}
