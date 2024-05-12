#include "text.h"
#include "utf8.h"

Text::Text(RenderMode render_mode, std::vector<FT_Glyph> unique_glyphs, std::vector<Glyph> glyphs, FT_Vector pen, FT_BBox bbox) {
	this->render_mode = render_mode;
	this->unique_glyphs = unique_glyphs;
	this->glyphs = glyphs;
	this->bbox = bbox;
	this->pen = pen;
}

Text::Text(Text&& text) noexcept {
	render_mode = text.render_mode;
	unique_glyphs = std::move(text.unique_glyphs);
	glyphs = std::move(text.glyphs);
	pen = text.pen;
	bbox = text.bbox;
}

Text::~Text() {
	for (FT_Glyph glyph : unique_glyphs) FT_Done_Glyph(glyph);
}