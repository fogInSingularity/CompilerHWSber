%option c++

%option noyywrap
%option yylineno
%option warn 
%option yyclass="slr::lex::Lexer::LexerImpl"

%{ 
#include <iostream>
#include <string>

#include "lexer/lexer_impl.hpp"
#include "lexer/lexer.hpp"
#include "helpers/common.hpp"

using namespace slr::lex;
%}

WS              [ \t\v]+
NLINE           [\n]+
NUMBER          [1-9][0-9]*|[0]
VAR             [a-zA-Z_]+

%%

{NLINE} { /* new_line(); */ };
{WS} {};
"//".* /* skip comments */

{NUMBER} { 
    new_token = Token{Token::TokenType::kNum, std::stoi(YYText())}; 
    return hlp::FromEnum(FlexParseStatus::kOk); 
};

"(" { 
    new_token = Token{Token::TokenType::kOp, Operations::kLeftBracket}; 
    return hlp::FromEnum(FlexParseStatus::kOk); 
};

")" { 
    new_token = Token{Token::TokenType::kOp, Operations::kRightBracket}; 
    return hlp::FromEnum(FlexParseStatus::kOk); 
};

"+" { 
    new_token = Token{Token::TokenType::kOp, Operations::kPlus}; 
    return hlp::FromEnum(FlexParseStatus::kOk); 
};

"-" { 
    new_token = Token{Token::TokenType::kOp, Operations::kMinus}; 
    return hlp::FromEnum(FlexParseStatus::kOk); 
};

"/" {
    new_token = Token{Token::TokenType::kOp, Operations::kDiv}; 
    return hlp::FromEnum(FlexParseStatus::kOk); 
};

"*" {
    new_token = Token{Token::TokenType::kOp, Operations::kMul}; 
    return hlp::FromEnum(FlexParseStatus::kOk); 
};

{VAR} {
    new_token = Token{Token::TokenType::kVar, YYText()}; 
    return hlp::FromEnum(FlexParseStatus::kOk); 
}; 

<<EOF>> {
    new_token = Token{Token::TokenType::kFinished, std::monostate{}}; 
    return hlp::FromEnum(FlexParseStatus::kOk); 
}

. {
    return hlp::FromEnum(FlexParseStatus::kFailed); 
} // error

%%
