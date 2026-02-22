#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <fstream>
#include <optional>
#include <string>
#include <variant>
#include <memory>
#include <cassert>

namespace slr {
namespace lex {

enum class Operations {
    kPlus,
    kMinus,
    kDiv,
    kMul,
    kLeftBracket,
    kRightBracket,
};

struct Token {
    enum class TokenType {
        kOp,
        kVar,
        kNum,
        kFinished,
    };

    TokenType type;

    std::variant<
        Operations,
        int64_t,
        std::string, // var's
        std::monostate // finished
    > val;

    std::string ToStr() {
        using enum TokenType;
        switch (type) {
            case kOp: {
                using enum Operations;
                switch (std::get<Operations>(val)) {
                    case kPlus: return "+";
                    case kMinus: return "-";
                    case kDiv: return "/";
                    case kMul: return "*";
                    case kLeftBracket: return "(";
                    case kRightBracket: return ")";
                }
            }
            case kVar: return get<std::string>(val);
            case kNum: return std::to_string(get<int64_t>(val));
            case kFinished: return "finished";
            default:
                assert(!"Unknown token type");
                return "<unknown>";
        }
    }
};

class Lexer {
  private:
    std::unique_ptr<std::ifstream> text_stream_;

    struct LexerImpl;
    std::unique_ptr<LexerImpl> impl_;

    bool is_finished_;
  public:
    Lexer(const std::string& text_path);
    ~Lexer();

    Lexer(Lexer&& other) noexcept;
    Lexer& operator=(Lexer&& other) noexcept;

    Token Next();
};

} // namespace lex
} // namespace slr

#endif // LEXER_HPP_
