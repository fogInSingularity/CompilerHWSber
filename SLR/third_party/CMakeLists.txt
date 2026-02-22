include(FetchContent)

FetchContent_Declare(
    spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.15.0
        GIT_SHALLOW true
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.14.0
        GIT_SHALLOW true
)
FetchContent_MakeAvailable(googletest)
