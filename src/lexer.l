%{
#include <iostream>
#include <fstream>
#include <map>
FlexLexer* lexer = nullptr;
std::map<std::string, std::string> keywords = {
    {"if", "IF"},
    {"loop", "LOOP"},
    {"int", "INT"},
    {"return", "RETURN"},
};
%}

%option c++
%option noyywrap

%%

[a-zA-Z_]+[a-zA-Z_0-9]* {
    if(keywords.contains(yytext)) {
        std::cout<<keywords[yytext]<<"\n";
    }
    else {
        std::cout<< "IDENTIFIER(" << yytext <<")\n";
    }
}
[0-9]+ {
    std::cout << "NUMBER(" << yytext << ")\n"; 
}
"->" {
    std::cout << "ARROW\n";
}
"+" { 
    std::cout << "PLUS\n"; 
}
"-" {
    std::cout << "MINUS\n"; 
}
"*" {
    std::cout << "MULTIPLY\n"; 
}
"/" {
    std::cout << "DIVIDE\n"; 
}
"=" {
    std::cout << "ASSIGNMENT\n";
}
"(" {
    std::cout << "LPAREN\n"; 
}
")" {
    std::cout << "RPAREN\n"; 
}
"{" {
    std::cout << "LBRACE\n"; 
}
"}" {
    std::cout << "RBRACE\n"; 
}
"," {
    std::cout << "COMMA\n";
}
":" {
    std::cout << "COLON\n";
}
";" {
    std::cout << "SEMICOLON\n";
}
[ \t\n]+ {
    /* Ignore whitespace */ 
}

.   {
    std::cout << "UNKNOWN TOKEN: " << yytext << "\n"; 
}

%%

int main(int argc, char** argv) {
    std::istream* in;  // Default input stream (stdin)
    std::ifstream* file;
    if (argc > 1) {
        // If a file is provided, open it and set as input stream
        file = new std::ifstream(argv[1]);
        if (!file->is_open()) {
            std::cerr << "Cannot open file: " << argv[1] << "\n";
            return 1;
        } 
        in = file;
    } else {
        std::cerr << "No file input given\n";
        return 0;
    }

    // Create a lexer object
    lexer = new yyFlexLexer(in);

    // Call the lexer
    while (lexer->yylex() != 0) {
    }

    delete lexer;
    delete file;
    return 0;
}
