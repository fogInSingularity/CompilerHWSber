#ifndef FLEXER_IMPL_HPP_
#define FLEXER_IMPL_HPP_

//#include "FlexLexer.h"

#include "lexer/lexer.hpp"

namespace slr {
namespace lex {

enum class FlexParseStatus {
    kOk,
    kFailed,
    kFinished,
};

// FlexLexer.h should not be included because it will lead to double definition of yyFlexLexer in flexer.l
struct Lexer::LexerImpl : public yyFlexLexer {
    using yyFlexLexer::yyFlexLexer;
    int yylex() override;

    Token new_token;
};

} // namespace lex
} // namespace slr

#endif // FLEXER_IMPL_HPP_
