#include "tokens.hpp"
namespace Tokens {
  Token::Token(TokType type, std::string metadata) : type(type), metadata(metadata) {}
}
