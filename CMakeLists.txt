cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)

project(makev20 VERSION 1.0)

add_executable(makev20
                        src/makev20.c
                        src/parser/argument_parser.c
                        src/parser/parser_execeptions.c)

target_include_directories(makev20 PRIVATE src/parser)
