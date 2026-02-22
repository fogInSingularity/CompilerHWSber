#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "parser/parser.hpp"

namespace {

class TempFile {
  public:
    explicit TempFile(const std::string& text) {
        const auto dir = std::filesystem::temp_directory_path();
        const auto name = "slr_parser_test_" + std::to_string(++counter_);
        path_ = dir / name;

        std::ofstream out{path_};
        out << text;
    }

    ~TempFile() {
        std::error_code ec;
        std::filesystem::remove(path_, ec);
    }

    const std::filesystem::path& Path() const {
        return path_;
    }

  private:
    std::filesystem::path path_;
    inline static size_t counter_ = 0;
};

class CoutRedirect {
  public:
    CoutRedirect() : old_{std::cout.rdbuf(buffer_.rdbuf())} {
    }
    ~CoutRedirect() {
        std::cout.rdbuf(old_);
    }

  private:
    std::ostringstream buffer_;
    std::streambuf* old_;
};

bool AcceptText(const std::string& text) {
    TempFile file{text};
    slr::prs::Parser parser{file.Path().string()};

    CoutRedirect guard;
    return parser.Accept();
}

} // namespace

TEST(ParserAccept, SimpleNumber) {
    EXPECT_TRUE(AcceptText("1"));
}

TEST(ParserAccept, VariableAndOps) {
    EXPECT_TRUE(AcceptText("a + b * (c - 3)"));
}

TEST(ParserAccept, WhitespaceAndNewlines) {
    EXPECT_TRUE(AcceptText("  1 + 2\n * 3 "));
}

TEST(ParserReject, TrailingOperator) {
    EXPECT_FALSE(AcceptText("1 +"));
}

TEST(ParserReject, MissingRightParen) {
    EXPECT_FALSE(AcceptText("(1 + 2"));
}

TEST(ParserReject, DoubleOperator) {
    EXPECT_FALSE(AcceptText("1 + * 2"));
}

TEST(ParserReject, LeadingRightParen) {
    EXPECT_FALSE(AcceptText(") 1"));
}
