#pragma once
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

enum TextAlignment {
	Left,
	Center,
	Right,
	//Justify,
};
enum RenderMode : char {
	Normal,
	LCD,
};

struct Glyph {
	FT_BitmapGlyph glyph;
	FT_Vector position;
};

class Text {
	std::vector<FT_Glyph> unique_glyphs;
public:
	RenderMode render_mode;
	std::vector<Glyph> glyphs;
	FT_Vector pen;
	FT_BBox bbox;

	Text(RenderMode render_mode, std::vector<FT_Glyph> unique_glyphs, std::vector<Glyph> glyphs, FT_Vector pen, FT_BBox bbox);
	Text(Text&& text) noexcept;

	Text(const Text&) = delete;
	Text& operator=(Text const&) = delete;

	~Text();
};