cmake_minimum_required(VERSION 3.8)

project(op_s2)

add_executable(lab1 "src/lab1_cli.cpp")
add_executable(lab2 "src/lab2_cli2.cpp")
add_executable(lab3 "src/lab3_bookshop.cpp")
add_executable(lab4 "src/lab4_bookshop2.cpp")
add_executable(lab5 "src/lab5_string.cpp")
add_executable(lab6 "src/lab6_bmp.cpp")
add_subdirectory(src/lab7_8_text_rendering)

set_property(TARGET lab1 PROPERTY CXX_STANDARD 20)
set_property(TARGET lab2 PROPERTY CXX_STANDARD 20)
set_property(TARGET lab3 PROPERTY CXX_STANDARD 20)
set_property(TARGET lab4 PROPERTY CXX_STANDARD 20)
set_property(TARGET lab5 PROPERTY CXX_STANDARD 20)
set_property(TARGET lab6 PROPERTY CXX_STANDARD 20)
