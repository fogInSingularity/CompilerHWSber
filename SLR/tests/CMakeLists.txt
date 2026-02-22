enable_testing()

add_executable(parser_tests)

target_sources(parser_tests
    PRIVATE
        parser_tests.cpp
)

target_link_libraries(parser_tests
    PRIVATE
        GTest::gtest_main
        slr-flags
        parser
        lexer
        helpers
)

include(GoogleTest)
gtest_discover_tests(parser_tests)
