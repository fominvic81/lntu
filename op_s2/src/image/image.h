#pragma once
#include <cmath>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "color.h"
#include "../text/font.h"
#include "../text/text.h"
#include "string.h"

struct UniformColor {
	Color24 color;
	UniformColor(Color24 color = { 0, 0, 0 }) { this->color = color; }
	UniformColor(Color color) { this->color = Color24{ color.r, color.g, color.b }; }

	Color24 operator()() { return color; };
	Color24 operator()(int, int) { return color; };
};

struct LinearGradient {
	struct Dir { float x, y; };

	std::vector<Color24> colors;
	int width;
	Dir dir;
	int offset;

	LinearGradient(const std::vector<Color24> colors, int width = 50, Dir dir = { 1.0f, 0.0f }, int offset = 0) {
		float len = sqrt(dir.x * dir.x + dir.y * dir.y);
		dir.x /= len; dir.y /= len;
		this->dir = dir;
		this->width = width;
		this->colors = colors;
		this->offset = offset;
	}

	Color24 operator()(int x, int y) {
		float projection = (dir.x * x + dir.y * y + offset) / width;
		float floored = floor(projection);
		int i = (int)floored % colors.size();
		Color colorA = colors[(i + 1) % colors.size()];
		Color colorB = colors[i];

		int k = (projection - floored) * 256;

		return {
			colorA.r * k / 255 + colorB.r * (255 - k) / 255,
			colorA.g * k / 255 + colorB.g * (255 - k) / 255,
			colorA.b * k / 255 + colorB.b * (255 - k) / 255,
		};
	};
};

class Image {
public:
	int width;
	int height;
	Color24* data;

	Image(int w, int h);
	Image(int w, int h, Color24* data);

	Image(Image&& rls) noexcept;

	Image(const Image&) = delete;
	Image& operator=(Image const&) = delete;

	~Image();

	void setPixel(int x, int y, Color color);
	Color getPixel(int x, int y) const;

	void forEach(Color(*predicate)(int, int, Image&)); // TODO: remove??

	template<class Pred>
	void fill(Pred pred);
	void fill(Color color);

	template<class Pred>
	void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Pred pred);
	void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color);

	template<class Pred>
	void fillCircle(int x0, int y0, int radius, Pred pred);
	void fillCircle(int x0, int y0, int radius, Color color);

	template<class Pred>
	void fillRect(int x0, int y0, int width, int height, float angle, Pred pred);
	void fillRect(int x0, int y0, int width, int height, float angle, Color color);

	template<class Pred>
	void drawLine(int x1, int y1, int x2, int y2, int width, Pred pred);
	void drawLine(int x1, int y1, int x2, int y2, int width, Color color);

	template<class Pred>
	Text& drawText(Text& text, int x, int y, Pred pred);
	Text& drawText(Text& text, int x, int y, Color color);

	template<class Pred>
	Text drawText(
		Font& font,
		int x,
		int y,
		const char* text_utf8,
		int size,
		Pred pred,
		int max_width = 0,
		TextAlignment alignment = TextAlignment::Left,
		RenderMode render_mode = RenderMode::LCD);
	Text drawText(
		Font& font,
		int x,
		int y,
		const char* text_utf8,
		int size,
		Color color,
		int max_width = 0,
		TextAlignment alignment = TextAlignment::Left,
		RenderMode render_mode = RenderMode::LCD);
};