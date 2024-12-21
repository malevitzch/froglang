#pragma once

#include "tokens.hpp"
#include "parser.hpp"
#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif


class FrogLexer : public yyFlexLexer
{
public:
    int yylex(value_type *const yylval);
    FrogLexer(std::istream* input)
        : yyFlexLexer(input) {}
};
