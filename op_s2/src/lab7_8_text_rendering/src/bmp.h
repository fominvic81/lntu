#pragma once
#include "image.h"

class BMP {
	static const int header_size;
	static const int info_header_size;
public:

	static void write(const Image& img, const char path[]);
	static Image read(const char path[]);
};