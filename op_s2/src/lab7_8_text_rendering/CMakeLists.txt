cmake_minimum_required(VERSION 3.8)

project(text_rendering)

find_package(Freetype REQUIRED)

add_executable(text_rendering
	"src/main.cpp"
	"src/bmp.cpp"
	"src/color.cpp"
	"src/font.cpp"
	"src/image.cpp"
	"src/string.cpp"
	"src/text.cpp"
	"src/utf8.cpp"
)

configure_file("${CMAKE_SOURCE_DIR}/fonts/ubuntu.mono.ttf" "${CMAKE_CURRENT_BINARY_DIR}/fonts/ubuntu.mono.ttf" COPYONLY)

target_link_libraries(text_rendering Freetype::Freetype)

set_property(TARGET text_rendering PROPERTY CXX_STANDARD 20)