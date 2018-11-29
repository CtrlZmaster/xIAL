cmake_minimum_required(VERSION 3.9)
project(xIAL C)

set(CMAKE_C_STANDARD 99)

add_executable(sptree main.c loadfile.c loadfile.h main.h)
add_executable(ldfl-test loadfile-test.c loadfile.c loadfile.h)