#include <string>
namespace Tokens {
  enum class TokType {
    
    IDENTIFIER,
    NUMBER,

    ARROW,
    PLUS,
    MINUS,
    STAR, //multiply (but I think this is better since it might mean deref later so probably worth to do this way)
    DIVISION,
    ASSIGNMENT,

    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    COMMA,
    COLON,
    SEMICOLON,

    UNKNOWN,

  };
//TODO: add keywords, perhaps even use inheritance for tokens

  struct Token {
    TokType type;
    std::string metadata;
  };

}
