cmake_minimum_required(VERSION 3.1)

project(doubly_linked_list LANGUAGES C)

add_library(doubly_linked_list
    STATIC
    ${PROJECT_SOURCE_DIR}/src/doubly_linked_list)

target_include_directories(doubly_linked_list
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include)

add_executable(driver ${PROJECT_SOURCE_DIR}/examples/driver.c)

target_link_libraries(driver PRIVATE doubly_linked_list)
