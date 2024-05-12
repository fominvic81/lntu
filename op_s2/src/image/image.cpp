#include <algorithm>
#include <iostream>

#include "image.h"
#include "../text/utf8.h"

Image::Image(int w, int h) {
	width = w;
	height = h;
	data = new Color24[w * h];
	std::fill(data, data + w * h, Color24{ 0, 0, 0 });
}
Image::Image(int w, int h, Color24* data) {
	width = w;
	height = h;
	this->data = data;
}

Image::Image(Image&& rls) noexcept {
	width = rls.width;
	height = rls.height;
	data = rls.data;
	rls.data = nullptr;
}

Image::~Image() {
	delete[] data;
}

void Image::setPixel(int x, int y, Color color) {
	if (x < 0 || x >= width || y < 0 || y >= height) { std::cerr << "Set pixel: pixel position is outside of bounds"; exit(1); }
	data[y * width + x] = Color24(color.r, color.g, color.b);
}

Color Image::getPixel(int x, int y) const {
	if (x < 0 || x >= width || y < 0 || y >= height) { std::cerr << "Get pixel: pixel position is outside of bounds"; exit(1); }
	return Color(data[y * width + x]);
}

void Image::forEach(Color(*predicate)(int, int, Image&)) {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Color result = predicate(x, y, *this);
			data[y * width + x] = Color24(result.r, result.g, result.b);
		}
	}
}

template<class Pred>
void Image::fill(Pred pred) {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			data[y * width + x] = pred(x, y);
		}
	}
}
void Image::fill(Color color) { fill(UniformColor(color)); }

template<class Pred>
void Image::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Pred pred) {
	using namespace std;
	if ((y2 - y1) * (y1 - y3) >= 0) {
		swap(x1, x2);
		swap(y1, y2);
	} else if ((y2 - y3) * (y3 - y1) >= 0) {
		swap(x3, x2);
		swap(y3, y2);
	}
	if (y1 > y3) {
		swap(x1, x3);
		swap(y1, y3);
	}

	if (y1 == y3) {
		if (y1 < 0 || y1 >= height) return;
		for (int x = max(0, min({ x1, x2, x3 })); x < min(width - 1, max({ x1, x2, x3 })); ++x) data[y1 * width + x] = pred(x, y1);
		return;
	}
	if (y1 != y2) for (int y = max(0, y1); y <= min(height - 1, y2); ++y) {
		int sx = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		int ex = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
		if (sx > ex) swap(sx, ex);

		for (int x = max(0, sx); x <= min(width - 1, ex); ++x) {
			data[y * width + x] = pred(x, y);
		}
	}
	if (y2 != y3) for (int y = max(0, y2); y <= min(height - 1, y3); ++y) {
		int sx = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
		int ex = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
		if (sx > ex) swap(sx, ex);

		for (int x = max(0, sx); x <= min(width - 1, ex); ++x) {
			data[y * width + x] = pred(x, y);
		}
	}
}
void Image::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color) { fillTriangle(x1, y1, x2, y2, x3, y3, UniformColor(color)); }

template<class Pred>
void Image::fillCircle(int x0, int y0, int radius, Pred pred) {
	using namespace std;
	int sx = max(0, x0 - radius);
	int sy = max(0, y0 - radius);
	int ex = min(this->width - 1, x0 + radius);
	int ey = min(this->height - 1, y0 + radius);

	for (int y = sy; y <= ey; ++y) {
		for (int x = sx; x <= ex; ++x) {
			if ((x - x0) * (x - x0) + (y - y0) * (y - y0) > radius * radius) continue;
			data[y * width + x] = pred(x, y);
		}
	}
}
void Image::fillCircle(int x0, int y0, int radius, Color color) { fillCircle(x0, y0, radius, UniformColor(color)); }


template<class Pred>
void Image::fillRect(int x0, int y0, int width, int height, float angle, Pred pred) {
	using namespace std;
	float dx0 = width  * 0.5f;
	float dy0 = height * 0.5f;

	float dx1 = dx0 * cos(angle) - dy0 * sin(angle);
	float dy1 = dx0 * sin(angle) + dy0 * cos(angle);
	float dx2 = dx0 * cos(angle) + dy0 * sin(angle);
	float dy2 = dx0 * sin(angle) - dy0 * cos(angle);

	int x1 = x0 + dx1, y1 = y0 + dy1;
	int x2 = x0 + dx2, y2 = y0 + dy2;
	int x3 = x0 - dx1, y3 = y0 - dy1;
	int x4 = x0 - dx2, y4 = y0 - dy2;

	fillTriangle(x1, y1, x3, y3, x2, y2, pred);
	fillTriangle(x1, y1, x4, y4, x3, y3, pred);
}
void Image::fillRect(int x0, int y0, int width, int height, float angle, Color color) { fillRect(x0, y0, width, height, angle, UniformColor(color)); }

template<class Pred>
void Image::drawLine(int x1, int y1, int x2, int y2, int width, Pred pred) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	double invLen = 1 / std::sqrt(dx * dx + dy * dy);
	int nx1 = (int)( dy * invLen * ((width - 1) / 2));
	int ny1 = (int)(-dx * invLen * ((width - 1) / 2));
	int nx2 = (int)( dy * invLen * (width / 2));
	int ny2 = (int)(-dx * invLen * (width / 2));

	int xa = x1 + nx1;
	int ya = y1 + ny1;
	int xb = x1 - nx2;
	int yb = y1 - ny2;
	int xc = x2 + nx1;
	int yc = y2 + ny1;
	int xd = x2 - nx2;
	int yd = y2 - ny2;

	this->fillTriangle(xa, ya, xb, yb, xc, yc, pred);
	this->fillTriangle(xb, yb, xc, yc, xd, yd, pred);
}
void Image::drawLine(int x1, int y1, int x2, int y2, int width, Color color) { drawLine(x1, y1, x2, y2, width, UniformColor(color)); }

template <class Pred>
Text& Image::drawText(Text& text, int x0, int y0, Pred pred) {

	switch (text.render_mode) {
	case Normal:
		for (auto& [glyph, position] : text.glyphs) {
			auto* bitmap = &glyph->bitmap;
			for (unsigned int y = 0; y < bitmap->rows; ++y) {
				for (unsigned int x = 0; x < bitmap->width; ++x) {
					int ax = (position.x >> 6) + glyph->left + x + x0;
					int ay = (position.y >> 6) - glyph->top + y + y0;
					if (ax < 0 || ax >= width || ay < 0 || ay >= height) continue;

					int c = bitmap->buffer[y * bitmap->pitch + x + 0];

					Color bg = data[ay * width + ax];
					Color cl = (bg * Color(255 - c, 255 - c, 255 - c) + Color(c, c, c)) * pred(ax, ay) / 255;

					data[ay * width + ax] = Color24{ cl.r, cl.g, cl.b };
				}
			}
		}
		break;
	case LCD:
		for (auto& [glyph, position] : text.glyphs) {
			auto* bitmap = &glyph->bitmap;
			for (unsigned int y = 0; y < bitmap->rows; ++y) {
				for (unsigned int x = 0; x < bitmap->width / 3; ++x) {
					int ax = (position.x >> 6) + glyph->left + x + x0;
					int ay = (position.y >> 6) - glyph->top + y + y0;
					if (ax < 0 || ax >= width || ay < 0 || ay >= height) continue;

					int r = bitmap->buffer[y * bitmap->pitch + x * 3 + 0];
					int g = bitmap->buffer[y * bitmap->pitch + x * 3 + 1];
					int b = bitmap->buffer[y * bitmap->pitch + x * 3 + 2];

					Color bg = data[ay * width + ax];
					Color cl = (bg * Color(255 - r, 255 - g, 255 - b) + Color(r, g, b) * pred(ax, ay)) / 255;

					data[ay * width + ax] = Color24{ cl.r, cl.g, cl.b };
				}
			}
		}
		break;
	default:
		std::cerr << "Error: Unsuported render mode" << std::endl;
		break;
	}

	return text;
}
Text& Image::drawText(Text& text, int x0, int y0, Color color) { return drawText(text, x0, y0, UniformColor(color)); }

template <class Pred>
Text Image::drawText(Font& font, int x, int y, const char* text_utf8, int size, Pred pred, int max_width, TextAlignment alignment, RenderMode render_mode) {
	if (max_width == 0) max_width = width - x;
	Text text = font.render(text_utf8, size, max_width, alignment, render_mode);

	drawText(text, x, y, pred);

	return text;
}
Text Image::drawText(Font& font, int x, int y, const char* text_utf8, int size, Color color, int max_width, TextAlignment alignment, RenderMode render_mode) {
	return drawText(font, x, y, text_utf8, size, UniformColor(color), max_width, alignment, render_mode);
}

template void Image::fill<UniformColor>(UniformColor);
template void Image::fillTriangle<UniformColor>(int, int, int, int, int, int, UniformColor);
template void Image::fillCircle<UniformColor>(int, int, int, UniformColor);
template void Image::fillRect<UniformColor>(int, int, int, int, float, UniformColor);
template void Image::drawLine<UniformColor>(int, int, int, int, int, UniformColor);
template Text& Image::drawText<UniformColor>(Text&, int, int, UniformColor);
template Text Image::drawText<UniformColor>(Font&, int, int, const char*, int, UniformColor, int, TextAlignment, RenderMode);

template void Image::fill<LinearGradient>(LinearGradient);
template void Image::fillTriangle<LinearGradient>(int, int, int, int, int, int, LinearGradient);
template void Image::fillCircle<LinearGradient>(int, int, int, LinearGradient);
template void Image::fillRect<LinearGradient>(int, int, int, int, float, LinearGradient);
template void Image::drawLine<LinearGradient>(int, int, int, int, int, LinearGradient);
template Text& Image::drawText<LinearGradient>(Text&, int, int, LinearGradient);
template Text Image::drawText<LinearGradient>(Font&, int, int, const char*, int, LinearGradient, int, TextAlignment, RenderMode);
