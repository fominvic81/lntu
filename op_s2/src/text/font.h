#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

#include "text.h"

class Font {
public:
	FT_Library library;
	FT_Face face;

	Font(const char path[]);
	Font(const unsigned char* buffer, size_t size);

	Font(const Font&) = delete;
	Font& operator=(Font const&) = delete;

	~Font();

	FT_Vector getSpaceAdvance(int size);
	Text render(const char text_utf8[], int size, int max_width, TextAlignment alignment, RenderMode render_mode);

	static void toStatic(const char* name, const char path[]);
	static void toStatic(const char* name, const char path[], const char outpath[]);
};

#define loadFontStatic(name) name ## _buffer, name ## _size