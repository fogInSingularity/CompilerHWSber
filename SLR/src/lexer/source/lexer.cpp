#include "lexer/lexer.hpp"

#include <cstddef>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <optional>
#include <cassert>

#include "FlexLexer.h"

#include "lexer/lexer_impl.hpp"
#include "helpers/common.hpp"

namespace slr {
namespace lex {

Lexer::Lexer(const std::string& text_path) 
    : text_stream_{std::make_unique<std::ifstream>(text_path)}, 
    impl_{std::make_unique<LexerImpl>(text_stream_.get())},
    is_finished_{false}
{}
Lexer::~Lexer() = default;

Lexer::Lexer(Lexer&& other) noexcept = default;
Lexer& Lexer::operator=(Lexer&& other) noexcept = default;

Token Lexer::Next() {
    FlexParseStatus status = hlp::ToEnum<FlexParseStatus>(impl_->yylex());
    switch (status) {
        case FlexParseStatus::kOk:
            return impl_->new_token;
        case FlexParseStatus::kFailed:
            throw std::runtime_error{"Lexer failed to parse"};
        default:
            assert(!"Unknown status");
    }
}

} // namespace lex
} // namespace slr
