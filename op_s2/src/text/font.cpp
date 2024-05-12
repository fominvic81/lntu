#include <iostream>
#include <unordered_map>
#include <locale>
#include <fstream>

#include "utf8.h"
#include "font.h"

static bool isBreakable(int a, int b) {
#ifdef _WIN32
	std::locale loc("en_US.UTF-8"); // unicode
	return !(std::isalnum(a, loc) && std::isalnum(b, loc));
#else // _WIN32
	return !(std::isalnum(a) && std::isalnum(b));
#endif
}

static void expandBBox(FT_BBox* target, const FT_BBox& by) {
	if (by.xMin < target->xMin) target->xMin = by.xMin;
	if (by.yMin < target->yMin) target->yMin = by.yMin;
	if (by.xMax > target->xMax) target->xMax = by.xMax;
	if (by.yMax > target->yMax) target->yMax = by.yMax;
}

Font::Font(const char path[]) {
	FT_Error error = FT_Init_FreeType(&library);
	if (error) {
		std::cerr << "Error: Failed to init freetype. Code: " << error << std::endl;
		exit(1);
	}
	error = FT_New_Face(library, path, 0, &face);
	if (error) {
		std::cerr << "Error: Failed to create face. Code: " << error << std::endl;
		exit(1);
	}
}
Font::Font(const unsigned char* buffer, size_t size) {
	FT_Error error = FT_Init_FreeType(&library);
	if (error) {
		std::cerr << "Error: Failed to init freetype. Code: " << error << std::endl;
		exit(1);
	}
	error = FT_New_Memory_Face(library, (const FT_Byte*)buffer, size, 0, &face);
	if (error) {
		std::cerr << "Error: Failed to create face. Code: " << error << std::endl;
		exit(1);
	}
}

Font::~Font() {
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

FT_Vector Font::getSpaceAdvance(int size) {
	FT_Error error = FT_Set_Char_Size(face, 0, size << 6, 72, 72);
	if (error) std::cerr << "Error: Failed to set char size to " << size << ". Code: " << error << std::endl;

	error = FT_Load_Char(face, ' ', FT_LOAD_DEFAULT);
	if (error) {
		std::cerr << "Error: Failed to load space glyph. Code: " << error << std::endl;
		return {};
	}

	return face->glyph->advance;
}

Text Font::render(const char text_utf8[], int size, int max_width, TextAlignment alignment, RenderMode render_mode) {
	FT_Error error = FT_Set_Char_Size(face, 0, size << 6, 72, 72);
	if (error) std::cerr << "Error: Failed to set char size to " << size << ". Code: " << error << std::endl;
	auto text = UTF8::decode(text_utf8);
	//for (unsigned int& c : text) if (c == ' ') c = U'\u00b7';

	size_t len = text.size();
	text.push_back(0);

	FT_Render_Mode ft_render_mode = render_mode == RenderMode::LCD ? FT_RENDER_MODE_LCD : FT_RENDER_MODE_NORMAL;
	std::unordered_map<int, FT_Glyph> glyph_by_unicode;
	std::vector<FT_Glyph> unique_glyphs;

	for (size_t i = 0; i < len; ++i) {
		int unicode = text[i];
		int character = FT_Get_Char_Index(face, unicode);

		if (glyph_by_unicode.find(unicode) == glyph_by_unicode.end()) {
			error = FT_Load_Glyph(face, character, FT_LOAD_DEFAULT);
			if (error) { std::cerr << "Error: Failed to load glyph " << character << ". Code: " << error << std::endl; continue; }

			FT_Glyph newGlyph;
			error = FT_Get_Glyph(face->glyph, &newGlyph);
			if (error) { std::cerr << "Error: Failed to get glyph. Code: " << error << std::endl; continue; }

			error = FT_Glyph_To_Bitmap(&newGlyph, ft_render_mode, 0, true);
			if (error) { std::cerr << "Error: Failed to convert glyph to bitmap. Code: " << error << std::endl; continue; }

			glyph_by_unicode.insert({ unicode, newGlyph });
			unique_glyphs.push_back(newGlyph);
		}
	}


	FT_Bool has_kerning = FT_HAS_KERNING(face);
	FT_Pos advance_new_line = face->size->metrics.height;
	FT_Vector advance_space = getSpaceAdvance(size);
	FT_Pos spaces_in_tab = 4;
	FT_Pos tab_col_width = advance_space.x * spaces_in_tab;

	FT_Pos bbox_inv_scale_x = render_mode == RenderMode::LCD ? 3 : 1;

	FT_BBox line_bbox = {};
	FT_BBox lookahead_bbox = {};
	FT_BBox text_bbox = {};

	text_bbox.xMin = text_bbox.yMin = 3200000;
	text_bbox.xMax = text_bbox.yMax = -3200000;
	line_bbox = text_bbox;

	std::vector<Glyph> glyphs;
	FT_Vector pen = { 0, advance_new_line };

	int prev = 0;
	int wordsInLine = 0;
	size_t line_start = 0;
	bool break_line_next = false;
	for (size_t i = 0; i <= len; ++i) { // less or equal is intentional

		std::vector<std::pair<FT_Glyph, FT_Vector>> word_glyphs; // glyph + kerning
		bool break_line = break_line_next;
		break_line_next = false;
		lookahead_bbox = line_bbox;

		size_t j = i;
		FT_Vector lookahead_pen = pen;
		for (; j < len; ++j) {
			// TODO: Make list of hidden characters
			if (text[j] == '\n') {
				prev = 0;
				break_line_next = true;
				break;
			}
			if (text[j] == '\t') {
				prev = 0;
				break;
			}

			auto glyph_p = glyph_by_unicode.find(text[j]);
			if (glyph_p == glyph_by_unicode.end()) continue;
			FT_Glyph glyph = (*glyph_p).second;

			int character = FT_Get_Char_Index(face, text[j]);

			if (has_kerning && prev) {
				FT_Vector kerning;
				error = FT_Get_Kerning(face, prev, character, FT_KERNING_DEFAULT, &kerning);
				if (error) std::cerr << "Error: Failed to get kerning. Code: " << error << std::endl;

				lookahead_pen.x += kerning.x;
				lookahead_pen.y += kerning.y;

				word_glyphs.push_back({ glyph, kerning });
			} else {
				word_glyphs.push_back({ glyph, {} });
			}

			FT_BBox glyph_bbox;
			FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_GRIDFIT, &glyph_bbox);

			glyph_bbox.xMax = glyph_bbox.xMin + (glyph_bbox.xMax - glyph_bbox.xMin) / bbox_inv_scale_x + lookahead_pen.x;
			expandBBox(&lookahead_bbox, glyph_bbox);

			if (lookahead_bbox.xMax > (max_width << 6)) break_line = true;

			lookahead_pen.x += glyph->advance.x >> 10;
			lookahead_pen.y += glyph->advance.y >> 10;
			prev = character;

			if (isBreakable(text[j], text[j + 1])) break;
		}
		++wordsInLine;

		if ((break_line && wordsInLine > 1) || i == len) {
			if (i != len) {
				pen.x = 0;
				pen.y += advance_new_line;
			}
			wordsInLine = 1;
			prev = 0;

			int width = std::max((FT_Pos)0, line_bbox.xMax - line_bbox.xMin);

			FT_Pos shiftX =
				alignment == TextAlignment::Left ? 0 :
				alignment == TextAlignment::Center ? ((max_width << 6) - width) / 2 :
				alignment == TextAlignment::Right ? ((max_width << 6) - width) : 0;

			for (size_t k = line_start; k < glyphs.size(); ++k) {
				glyphs[k].position.x += shiftX;
			}
			if (i == len) pen.x += shiftX;
			line_bbox.xMin += shiftX;
			line_bbox.xMax += shiftX;

			line_start = glyphs.size();
			expandBBox(&text_bbox, line_bbox);
			line_bbox.xMin = line_bbox.yMin = 3200000;
			line_bbox.xMax = line_bbox.yMax = -3200000;
		}
		if (text[j] == '\t') pen.x = (pen.x / tab_col_width + 1) * tab_col_width;

		for (auto& [glyph, kerning] : word_glyphs) {
			pen.x += kerning.x;
			pen.y += kerning.y;

			FT_BBox unscaled_bbox;
			FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_GRIDFIT, &unscaled_bbox);

			FT_BBox glyph_bbox = {};

			// divide by three because of lcd
			glyph_bbox.xMin = unscaled_bbox.xMin + pen.x;
			glyph_bbox.xMax = unscaled_bbox.xMin + (unscaled_bbox.xMax - unscaled_bbox.xMin) / bbox_inv_scale_x + pen.x;
			glyph_bbox.yMin = -unscaled_bbox.yMax + pen.y;
			glyph_bbox.yMax = -unscaled_bbox.yMin + pen.y;
			expandBBox(&line_bbox, glyph_bbox);

			glyphs.push_back({ (FT_BitmapGlyph)glyph, pen });

			pen.x += glyph->advance.x >> 10;
			pen.y += glyph->advance.y >> 10;
		}
		i = j;
	}

	return Text(render_mode, unique_glyphs, glyphs, pen, text_bbox);
}

void Font::toStatic(const char* name, const char path[]) { toStatic(name, path, (std::string(path) + ".inc").c_str()); }

static char hex[] = "0123456789abcdef";
void Font::toStatic(const char* name, const char path[], const char outpath[]) {
	std::ifstream input(path, std::ios::binary);
	std::ofstream output(outpath);

	int size = 0;
	char c;
	output << "const unsigned char " << name << "_buffer[] = {";
	while (true) {
		input.read(&c, 1);
		if (input.eof() || !input.good()) break;
		char byte_lit[2] = { hex[(c >> 4) & 0xf], hex[c & 0xf] };

		output.write("0x", 2);
		output.write(byte_lit, 2);
		output.write(",", 1);
		++size;
	}
	output << "};\nconst size_t " << name << "_size = " << size << ";";
}