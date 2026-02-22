#include <iostream>

#include <optional>
#include <spdlog/spdlog.h>
#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "lexer/lexer.hpp"

int main(int argc, const char* argv[]) {
    auto logger = spdlog::basic_logger_mt("slr", "slr.log", true);
    spdlog::set_default_logger(logger);

    spdlog::set_pattern("[%l] %v"); // remove time and name(%n) from log

#if defined (NDEBUG)
    spdlog::set_level(spdlog::level::info);
#else // NDEBUG
    // spdlog::flush_on(spdlog::level::trace);
    spdlog::set_level(spdlog::level::debug);
#endif // NDEBUG

    // log argv
    for (int i = 0; i < argc; i++) {
        spdlog::info("argv[{}]: {}", i, argv[i]);
    }

    if (argc < 2) {
        std::cerr << 
            "ERROR: not enough args\n" 
            "   usage: " << argv[0]  << " <text_path>\n";

        return EXIT_FAILURE;
    }

    slr::lex::Lexer lexer{argv[1]};

    bool is_finished = false;
    do {
        slr::lex::Token cur_token = lexer.Next();
        is_finished = (cur_token.type == slr::lex::Token::TokenType::kFinished);

        if (!is_finished) {
            std::cout << cur_token.ToStr();
        }
    } while (!is_finished);
}
