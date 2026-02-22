cmake_minimum_required(VERSION 3.21)

project(SLR)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

add_subdirectory(flags)
add_subdirectory(third_party)

add_subdirectory(src/helpers)
add_subdirectory(src/lexer)
add_subdirectory(src/parser)

add_executable(slr.x)

target_sources(slr.x
    PRIVATE
        src/main.cpp
)

target_include_directories(slr.x
    PRIVATE
        include/
)

target_link_libraries(slr.x
    PRIVATE 
        slr-flags
        lexer
        parser

        spdlog
)

add_subdirectory(tests)
