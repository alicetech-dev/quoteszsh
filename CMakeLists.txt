cmake_minimum_required(VERSION 3.15)
project(quoteszsh) # O el nombre de tu proyecto

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
        cpphttplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG v0.15.3
)
FetchContent_MakeAvailable(cpphttplib)

add_executable(quoteszsh main.cpp)

target_link_libraries(quoteszsh PRIVATE
        nlohmann_json::nlohmann_json
        httplib::httplib
)

if (UNIX AND NOT APPLE)
    find_package(OpenSSL REQUIRED)
    find_package(Threads REQUIRED)
    target_link_libraries(quoteszsh PRIVATE
            OpenSSL::SSL
            OpenSSL::Crypto
            Threads::Threads
    )
endif()