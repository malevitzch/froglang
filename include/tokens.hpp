#pragma once 
#include <string>
#include "ast/node.hpp"
namespace Tokens {
  enum class TokType {

    TYPE_ID,
    IDENTIFIER,
    NUMBER,

    // Logical constants
    TRUE,
    FALSE,

    ARROW,
    PLUS,
    MINUS,
    STAR, //multiply (but I think this is better since it might mean deref later so probably worth to do this way)
    SLASH,
    ASSIGNMENT,

    LESS,
    GREATER,
    LESSEQ,
    GREATEREQ,
    EQUALITY,
    INEQUALITY,

    LOGICAL_AND,
    LOGICAL_OR,
    NEGATION,

    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    COMMA,
    COLON,
    SEMICOLON,

    RETURN,
    FUNCTION,

    IF,
    ELSE,
    LOOP, //replacement of a while loop
    FOR, //there should be a for loop cause it's convenient
    TIMES, //a type of loop that executes a set number of times

    UNKNOWN,

  };

  struct Token {
    TokType type;
    std::string metadata;
    Token(TokType type, std::string metadata);
    Token(TokType type);
    Token() = default;
  };
}
struct FrogTok {
  std::shared_ptr<Tokens::Token> token;
  std::shared_ptr<ast::Node> node;
};
