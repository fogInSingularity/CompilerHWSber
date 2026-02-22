#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <string>
#include <variant>
#include <vector>
#include <unordered_set>
#include <set>

#include "lexer/lexer.hpp"

namespace slr {
namespace prs {

class Parser {
  private:
    lex::Lexer lexer_;

  public:
    Parser(const std::string& text_path) : lexer_{text_path} {}
    bool Accept();
};

} // namespace prs
} // namespace slr

#endif // PARSER_HPP_
