cmake_minimum_required(VERSION 3.14)
project(archiver_c C)

set(CMAKE_C_STANDARD 99)

add_executable(archiver_c main.c coder.c priority_queue.c)