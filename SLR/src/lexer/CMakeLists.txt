add_library(lexer STATIC)

find_package(FLEX REQUIRED)

flex_target(flexer
    source/flexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/flexer.cpp
)

message(STATUS "Flex produced: ${FLEX_flexer_OUTPUTS}")

target_sources(lexer
    PRIVATE
        ${FLEX_flexer_OUTPUTS}
        source/lexer.cpp
)

target_include_directories(lexer
    PUBLIC
        ${FLEX_INCLUDE_DIRS}
        include
)

target_link_libraries(lexer
    PUBLIC
        slr-flags
        helpers
)
