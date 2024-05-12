cmake_minimum_required(VERSION 3.8)

project(op_s2)

find_package(Freetype)
set(string_sources "src/string/string.cpp")
set(image_sources "src/image/bmp.cpp" "src/image/color.cpp" "src/image/image.cpp")
set(text_sources "src/text/font.cpp" "src/text/text.cpp" "src/text/utf8.cpp")
set(all_sources ${string_sources} ${image_sources} ${text_sources})

add_executable(lab1 "src/lab1_cli.cpp")
add_executable(lab2 "src/lab2_cli2.cpp")
add_executable(lab3 "src/lab3_bookshop.cpp")
add_executable(lab4 "src/lab4_bookshop2.cpp")
add_executable(lab5 "src/lab5_string.cpp" ${string_sources})

add_executable(lab6 "src/lab6_bmp.cpp" ${all_sources})
target_link_libraries(lab6 Freetype::Freetype)

add_executable(lab7 "src/lab7_glyph_rendering.cpp" ${all_sources})
target_link_libraries(lab7 Freetype::Freetype)

add_executable(lab8 "src/lab8_text_rendering.cpp" ${all_sources})
target_link_libraries(lab8 Freetype::Freetype)

configure_file("${CMAKE_SOURCE_DIR}/fonts/ubuntu.mono.ttf" "${CMAKE_CURRENT_BINARY_DIR}/fonts/ubuntu.mono.ttf" COPYONLY)

set_property(TARGET lab1 PROPERTY CXX_STANDARD 20)
set_property(TARGET lab2 PROPERTY CXX_STANDARD 20)
set_property(TARGET lab3 PROPERTY CXX_STANDARD 20)
set_property(TARGET lab4 PROPERTY CXX_STANDARD 20)
set_property(TARGET lab5 PROPERTY CXX_STANDARD 20)
set_property(TARGET lab6 PROPERTY CXX_STANDARD 20)
set_property(TARGET lab7 PROPERTY CXX_STANDARD 20)
set_property(TARGET lab8 PROPERTY CXX_STANDARD 20)
