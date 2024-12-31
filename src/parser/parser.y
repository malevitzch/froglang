
%language "c++"
%define parse.error verbose
%define api.value.type {FrogTok}

%parse-param { FrogLexer& lexer }
%code requires { 
  #include <memory>
  #include "ast/node.hpp"
  #include "ast/expression_nodes.hpp"
  #include "froglexer.hpp"
  #include "tokens.hpp"
}
%code {
  extern std::shared_ptr<ast::Node> ast_root;
  #define yylex lexer.yylex
}

%{

%}
//TODO: finish inputting tokens
%token <token> NUMBER
%token <token> IDENTIFIER
%token <token> TYPE_ID

%token <token> ARROW

%token <token> PLUS
%token <token> MINUS
%token <token> STAR
%token <token> SLASH

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

%left PLUS MINUS
%left STAR SLASH

%type <node> expression program global_obj function

%%
program: /* */ {
    ast_root = std::make_shared<ast::ProgramNode>();
    std::cout<<"FINISHED\n";
  }
  | program global_obj {
    auto globject = std::dynamic_pointer_cast<ast::GlobjectNode>($2);
    auto prog_node = std::dynamic_pointer_cast<ast::ProgramNode>($1);
    prog_node->add_obj(globject);
    std::cout<<"Added globject to program\n";
    $$ = $1;
  }
  ;

global_obj: function {
    std::cout<<"FUNCTION\n";
    $$ = $1;
  }
  ;

function: function_declaration block {std::cout<<"FUNC\n";}
  ;

function_declaration: FUNCTION IDENTIFIER arglist ARROW TYPE_ID {std::cout<<"DECLARED function("<<$2->metadata<<")\n";}
  ;

block: LBRACE statements RBRACE {std::cout<<"BLOCK\n";}
  ;

statements: /* empty */
  | statement statements {std::cout<<"combined\n";}
  ;

statement: expression SEMICOLON {std::cout<<"STATEMENT\n";}
  | declaration SEMICOLON 
  | declaration ASSIGNMENT expression SEMICOLON {}
  | RETURN expression SEMICOLON {std::cout<<"RETURNED\n";}
  | RETURN {std::cout<<"RETURNED (void)\n";}
  ;

declaration: IDENTIFIER COLON TYPE_ID {std::cout<<"DECLARED\n";}
  ;

call_arglist: /* empty */
  | expression
  | expression COMMA call_arglist
  ;

expression: NUMBER {
    $$ = std::make_shared<ast::IntegerConstant>($1->metadata, "int32");
    std::cout<<"Converted\n";
  }
  | IDENTIFIER
  | IDENTIFIER LPAREN call_arglist RPAREN
  | LPAREN expression RPAREN {$$ = $2;}
  | expression PLUS expression {std::cout<<"Added\n";}
  | expression MINUS expression {std::cout<<"Subtracted\n";}
  | expression STAR expression {std::cout<<"Multiplied\n";}
  | expression SLASH expression {std::cout<<"Divided\n";}
  ;

args:  /*empty*/
  | declaration
  | declaration COMMA args
  ;

arglist: LPAREN args RPAREN {std::cout<<"FUNCTION ARGS\n";}
  ;
%%

void yy::parser::error(const std::string &message)
{
  std::cerr << "Error: " << message << std::endl;
}
