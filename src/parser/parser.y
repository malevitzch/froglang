
%language "c++"
%define parse.error verbose
%define api.value.type {FrogTok}

%parse-param { FrogLexer& lexer }
%code requires { 
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

%token <token> ARROW

%token <token> PLUS
%token <token> MINUS
%token <token> STAR
%token <token> SLASH

%token <token> LESS
%token <token> GREATER
%token <token> EQUALITY
%token <token> INEQUALITY

%token <token> LOGICAL_AND
%token <token> LOGICAL_OR

%token <token> ASSIGNMENT

%token <token> FUNCTION
%token <token> RETURN

%token <token> LPAREN
%token <token> RPAREN

%token <token> LBRACE
%token <token> RBRACE
%token <token> SEMICOLON
%token <token> COMMA
%token <token> COLON

%token <token> IF
%token <token> ELSE

%left LOGICAL_OR
%left LOGICAL_AND
%left LESS GREATER EQUALITY INEQUALITY
%left PLUS MINUS
%left STAR SLASH

%right UMINUS

%type <node>
  expression program global_obj function
  function_declaration block arglist
  statement statements declaration
  args call_args call_arglist

%%
program: /* */ {
    ast_root = std::make_shared<ast::ProgramNode>();
    $$ = ast_root;
    *diagnostic_stream<<"FINISHED\n";
  }
  | program global_obj {
    auto prog_node = dynamic_pointer_cast<ast::ProgramNode>($1);
    auto globject = dynamic_pointer_cast<ast::GlobjectNode>($2);
    prog_node->add_obj(globject);
    *diagnostic_stream<<"Added globject to program"<<std::endl;
    $$ = $1;
  }
  ;

global_obj: function {
    *diagnostic_stream<<"FUNCTION\n";
    $$ = dynamic_pointer_cast<ast::GlobjectNode>($1);
  }
  ;

function: function_declaration block {
    auto decl = std::dynamic_pointer_cast<ast::FunctionDeclaration>($1);
    auto body = std::dynamic_pointer_cast<ast::Block>($2);
    $$ = std::make_shared<ast::FunctionGlobject>(decl, body);
  }
  ;

function_declaration: FUNCTION IDENTIFIER arglist ARROW TYPE_ID {
    $$ = std::make_shared<ast::FunctionDeclaration>($2->metadata, dynamic_pointer_cast<ast::FunctionArglist>($3), CompilerContext::Types->get_type($5->metadata));
    *diagnostic_stream<<"DECLARED function("<<$2->metadata<<")\n";
  }
  | FUNCTION IDENTIFIER arglist {
    $$ = std::make_shared<ast::FunctionDeclaration>($2->metadata, dynamic_pointer_cast<ast::FunctionArglist>($3), llvm::Type::getVoidTy(*CompilerContext::TheContext));
  }
  ;

block: LBRACE statements RBRACE {
    auto statements = dynamic_pointer_cast<ast::Statements>($2);
    $$ = std::make_shared<ast::Block>(statements);
    *diagnostic_stream<<"BLOCK\n";
  }
  ;

statements: /* empty */ { 
    $$ = std::make_shared<ast::Statements>(); 
  }
  | statements statement {
    auto statements = dynamic_pointer_cast<ast::Statements>($1);
    auto statement = dynamic_pointer_cast<ast::StatementNode>($2); 
    statements->add_statement(statement);
    *diagnostic_stream<<"combined\n";
    $$ = $1;
  }
  ;

statement: expression SEMICOLON {
    $$ = std::make_shared<ast::ExpressionStatement>(dynamic_pointer_cast<ast::ExprNode>($1));
    *diagnostic_stream<<"Expression Statement\n";
  }
  | declaration SEMICOLON {
    $$ = std::make_shared<ast::DeclarationStatement>(dynamic_pointer_cast<ast::DeclarationNode>($1));
  }
  | declaration ASSIGNMENT expression SEMICOLON {
    auto decl = dynamic_pointer_cast<ast::DeclarationNode>($1);
    auto expr = dynamic_pointer_cast<ast::ExprNode>($3);
    $$ = std::make_shared<ast::DeclarationAssignmentStatement>(decl, expr);
  }
  | RETURN expression SEMICOLON {
    auto expr = dynamic_pointer_cast<ast::ExprNode>($2);
    $$ = std::make_shared<ast::ReturnStatement>(expr);
    *diagnostic_stream<<"RETURNED\n";
  }
  | RETURN SEMICOLON {
    $$ = std::make_shared<ast::ReturnStatement>();
    *diagnostic_stream<<"RETURNED (void)\n";
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
    $$ = std::make_shared<ast::DeclarationNode>(CompilerContext::Types->get_type($3->metadata), $1->metadata);
    *diagnostic_stream<<"DECLARED\n";
  }
  ;


call_arglist: LPAREN call_args RPAREN {
    auto call_args = dynamic_pointer_cast<ast::FunctionCallArgs>($2);
    $$ = std::make_shared<ast::FunctionCallArglist>(call_args); 
  }

call_args: /* empty */ {
    $$ = std::make_shared<ast::FunctionCallArgs>();
  }
  | expression {
    auto call_args = std::make_shared<ast::FunctionCallArgs>();
    auto expr = dynamic_pointer_cast<ast::ExprNode>($1);
    call_args->add_arg(expr);
    $$ = call_args;
  }
  | call_args COMMA expression {
    auto call_args = dynamic_pointer_cast<ast::FunctionCallArgs>($1);
    auto expr = dynamic_pointer_cast<ast::ExprNode>($3);
    call_args->add_arg(expr);
    $$ = call_args;
  }
  ;

expression: NUMBER {
    //TODO: solve the type issue
    $$ = std::make_shared<ast::IntegerConstant>($1->metadata, llvm::Type::getInt32Ty(*CompilerContext::TheContext));
    *diagnostic_stream<<"Converted\n";
  }
  | IDENTIFIER {
    $$ = std::make_shared<ast::VariableIdentifier>($1->metadata);
  }
  | MINUS expression %prec UMINUS {
    $$ = std::make_shared<ast::UnaryOperator>("-", dynamic_pointer_cast<ast::ExprNode>($2));
  }
  | IDENTIFIER call_arglist {
    auto call_arglist = dynamic_pointer_cast<ast::FunctionCallArglist>($2);
    $$ = std::make_shared<ast::FunctionCallExpr>($1->metadata, call_arglist);
  }
  | LPAREN expression RPAREN {$$ = $2;}
  | expression PLUS expression {
    $$ = std::make_shared<ast::BinaryOperator>("+", dynamic_pointer_cast<ast::ExprNode>($1), dynamic_pointer_cast<ast::ExprNode>($3));
    *diagnostic_stream<<"Added\n";
  }
  | expression MINUS expression {
    $$ = std::make_shared<ast::BinaryOperator>("-", dynamic_pointer_cast<ast::ExprNode>($1), dynamic_pointer_cast<ast::ExprNode>($3));
    *diagnostic_stream<<"Subtracted\n";
  }
  | expression STAR expression {
    $$ = std::make_shared<ast::BinaryOperator>("*", dynamic_pointer_cast<ast::ExprNode>($1), dynamic_pointer_cast<ast::ExprNode>($3));
    *diagnostic_stream<<"Multiplied\n";
  }
  | expression SLASH expression {
    $$ = std::make_shared<ast::BinaryOperator>("/", dynamic_pointer_cast<ast::ExprNode>($1), dynamic_pointer_cast<ast::ExprNode>($3));
    *diagnostic_stream<<"Divided\n";
  }
  | expression LESS expression {
    $$ = std::make_shared<ast::BinaryOperator>("<", dynamic_pointer_cast<ast::ExprNode>($1), dynamic_pointer_cast<ast::ExprNode>($3));
    *diagnostic_stream<<"Compared (less)\n";
  }
  | expression GREATER expression {
    $$ = std::make_shared<ast::BinaryOperator>(">", dynamic_pointer_cast<ast::ExprNode>($1), dynamic_pointer_cast<ast::ExprNode>($3));
    *diagnostic_stream<<"Compared (more)\n";
  }
  | expression EQUALITY expression {
    $$ = std::make_shared<ast::BinaryOperator>("==", dynamic_pointer_cast<ast::ExprNode>($1), dynamic_pointer_cast<ast::ExprNode>($3));
    *diagnostic_stream<<"Compared (equality)\n";
  }
  | expression INEQUALITY expression {
    $$ = std::make_shared<ast::BinaryOperator>("!=", dynamic_pointer_cast<ast::ExprNode>($1), dynamic_pointer_cast<ast::ExprNode>($3));
    *diagnostic_stream<<"Compared (inequality)\n";
  }
  | expression LOGICAL_AND expression {
    $$ = std::make_shared<ast::BinaryOperator>("&&", dynamic_pointer_cast<ast::ExprNode>($1), dynamic_pointer_cast<ast::ExprNode>($3));
    *diagnostic_stream<<"Logical AND\n";
  }
  | expression LOGICAL_OR expression {
    $$ = std::make_shared<ast::BinaryOperator>("||", dynamic_pointer_cast<ast::ExprNode>($1), dynamic_pointer_cast<ast::ExprNode>($3));
    *diagnostic_stream<<"Logical OR\n";
  }
  ;

args: /* empty */ {
    $$ = std::make_shared<ast::FunctionArgs>();
  }
  | declaration {
    auto args = std::make_shared<ast::FunctionArgs>();
    auto arg = dynamic_pointer_cast<ast::DeclarationNode>($1);
    args->add_arg(arg);
    $$ = args;
  }
  | args COMMA declaration {
    auto args = dynamic_pointer_cast<ast::FunctionArgs>($1);
    auto arg = dynamic_pointer_cast<ast::DeclarationNode>($3);
    args->add_arg(arg);
    $$ = args;
  }
  ;

arglist: LPAREN args RPAREN {
    auto args = dynamic_pointer_cast<ast::FunctionArgs>($2);
    $$ = std::make_shared<ast::FunctionArglist>(args);
    *diagnostic_stream<<"FUNCTION ARGLIST\n";
  }
  ;
%%

void yy::parser::error(const std::string &message) {
  std::cerr << "Error: " << message << std::endl;
}
