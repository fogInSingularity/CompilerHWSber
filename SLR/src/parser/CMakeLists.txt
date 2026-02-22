add_library(parser STATIC)

target_sources(parser
    PRIVATE
        source/parser.cpp
)

target_include_directories(parser
    PUBLIC
        include
)

target_link_libraries(parser
    PUBLIC
        slr-flags
        helpers
        lexer
)
