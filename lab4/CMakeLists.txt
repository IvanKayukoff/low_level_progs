cmake_minimum_required(VERSION 3.9.6)
project(lab4 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-O0")

add_executable(lab4 linked_list.c linked_list.h main.c higher_order.c higher_order.h)