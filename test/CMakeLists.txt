cmake_minimum_required(VERSION 3.8)
set(This UriTests)

set(Sources
    UriTests.cpp
)

add_executable(${This} ${Sources})

target_link_libraries(${This} PUBLIC
    gtest_main
    Uri
)

add_test(
    NAME ${This}
    COMMAND ${This}
)