#include <cstdint>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;

class Color24 {
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;

	Color24() : r(0), g(0), b(0) {};
	Color24(int r, int g, int b) : r(r), g(g), b(b) {};
};

class Color {
public:
	int r;
	int g;
	int b;

	Color() : r(0), g(0), b(0) {};
	Color(int r, int g, int b) : r(r), g(g), b(b) {};
	Color(Color24 color) : r(color.r), g(color.g), b(color.b) {};

	Color operator+(Color& rls) const { return { r + rls.r, g + rls.g, b + rls.b }; }
	Color operator+(Color rls) const { return { r + rls.r, g + rls.g, b + rls.b }; }
	Color operator-(Color& rls) const { return { r - rls.r, g - rls.g, b - rls.b }; }
	Color operator-(Color rls) const { return { r - rls.r, g - rls.g, b - rls.b }; }
	Color operator*(int rls) const { return { r * rls, g * rls, b * rls }; }
	Color operator*(Color rls) const { return { r * rls.r, g * rls.g, b * rls.b }; }
	Color operator/(int rls) const { return { r / rls, g / rls, b / rls }; }
};

class Image {
public:
	int width;
	int height;
	Color24* data;

	Image(int w, int h) {
		width = w;
		height = h;
		data = new Color24[w * h];
		fill(data, data + w * h, Color24{ 0, 0, 0 });
	}
	Image(int w, int h, Color24* data) {
		width = w;
		height = h;
		this->data = data;
	}

	Image(Image&& rls) noexcept {
		width = rls.width;
		height = rls.height;
		data = rls.data;
		rls.data = nullptr;
	}

	Image(const Image& rls) = delete;
	Image& operator=(const Image& rls) = delete;

	~Image() {
		delete[] data;
	}

	void setPixel(int x, int y, Color color) {
		if (x < 0 || x >= width || y < 0 || y >= height) { cerr << "Set pixel: pixel position is outside of bounds"; exit(1); }
		data[y * width + x] = Color24(color.r, color.g, color.b);
	}

	Color getPixel(int x, int y) const {
		if (x < 0 || x >= width || y < 0 || y >= height) { cerr << "Get pixel: pixel position is outside of bounds"; exit(1); }
		return Color(data[y * width + x]);
	}

	void forEach(Color(*predicate)(int, int, Image&)) {
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				Color result = predicate(x, y, *this);
				data[y * width + x] = Color24(result.r, result.g, result.b);
			}
		}
	}

	/*void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color) {
		if ((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1) < 0) return fillTriangle(x2, y2, x1, y1, x3, y3, color);

		int sx = max(0, min({ x1, x2, x3 }));
		int sy = max(0, min({ y1, y2, y3 }));
		int ex = min(this->width - 1, max({ x1, x2, x3 }));
		int ey = min(this->height - 1, max({ y1, y2, y3 }));

		for (int x = sx; x <= ex; ++x) {
			for (int y = sy; y <= ey; ++y) {
				int dxa1 = x - x1;
				int dya1 = y - y1;
				int dxb1 = x2 - x1;
				int dyb1 = y2 - y1;
				if (dxa1 * dyb1 - dxb1 * dya1 > 0) continue;
				int dxa2 = x - x2;
				int dya2 = y - y2;
				int dxb2 = x3 - x2;
				int dyb2 = y3 - y2;
				if (dxa2 * dyb2 - dxb2 * dya2 > 0) continue;
				int dxa3 = x - x3;
				int dya3 = y - y3;
				int dxb3 = x1 - x3;
				int dyb3 = y1 - y3;
				if (dxa3 * dyb3 - dxb3 * dya3 > 0) continue;

				data[y * width + x] = Color24(color.r, color.g, color.b);
			}
		}
	}*/
	void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color) {
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
			for (int x = min({ x1, x2, x3 }); x < max({ x1, x2, x3 }); ++x) data[y1 * width + x] = Color24(color.r, color.g, color.b);
			return;
		}
		if (y1 != y2) for (int y = max(0, y1); y <= min(height - 1, y2); ++y) {
			int sx = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
			int ex = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
			if (sx > ex) swap(sx, ex);

			for (int x = max(0, sx); x <= min(width - 1, ex); ++x) {
				data[y * width + x] = Color24(color.r, color.g, color.b);
			}
		}
		if (y2 != y3) for (int y = max(0, y2); y <= min(height - 1, y3); ++y) {
			int sx = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
			int ex = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
			if (sx > ex) swap(sx, ex);

			for (int x = max(0, sx); x <= min(width - 1, ex); ++x) {
				data[y * width + x] = Color24(color.r, color.g, color.b);
			}
		}
	}

	void fillCircle(int x0, int y0, int radius, Color color) {
		int sx = max(0, x0 - radius);
		int sy = max(0, y0 - radius);
		int ex = min(this->width - 1, x0 + radius);
		int ey = min(this->height - 1, y0 + radius);

		for (int x = sx; x <= ex; ++x) {
			for (int y = sy; y <= ey; ++y) {
				if ((x - x0) * (x - x0) + (y - y0) * (y - y0) > radius * radius) continue;
				data[y * width + x] = Color24(color.r, color.g, color.b);
			}
		}
	}

	void drawLine(int x1, int y1, int x2, int y2, int width, Color color) {
		int dx = x2 - x1;
		int dy = y2 - y1;
		double invLen = 1 / sqrt(dx * dx + dy * dy);
		int nx1 = dy * invLen * ((width - 1) / 2);
		int ny1 = -dx * invLen * ((width - 1) / 2);
		int nx2 = dy * invLen * (width / 2);
		int ny2 = -dx * invLen * (width / 2);

		int xa = x1 + nx1;
		int ya = y1 + ny1;
		int xb = x1 - nx2;
		int yb = y1 - ny2;
		int xc = x2 + nx1;
		int yc = y2 + ny1;
		int xd = x2 - nx2;
		int yd = y2 - ny2;

		this->fillTriangle(xa, ya, xb, yb, xc, yc, color);
		this->fillTriangle(xb, yb, xc, yc, xd, yd, color);
	}
};

class BMP {
	static const int headerSize = 14;
	static const int infoHeaderSize = 40;
public:
	static void write(const Image& img, const char path[]) {
		ofstream file(path, ios::binary);

		int padding = (((img.width * 8 * 3) + 31) / 32 * 32 - img.width * 8 * 3) / 8;
		int rasterDataSize = (img.width * 3 + padding) * img.height;
		int fileSize = headerSize + infoHeaderSize + rasterDataSize;
		int dataOffset = headerSize + infoHeaderSize;
		int planes = 1;
		int bitsPerPixel = 8 * 3;

		const char* zero = "\0\0\0\0";

		// Header
		file.write("BM", 2); // Signature (2 bytes)
		file.write((char*)&fileSize, 4); // File size (4 bytes)
		file.write(zero, 4); // Reserved (4 bytes)
		file.write((char*)&dataOffset, 4); // Data offset (4 bytes)
		// Info header
		file.write((char*)&infoHeaderSize, 4); // Info header size (4 bytes)
		file.write((char*)&img.width, 4); // width (4 bytes)
		file.write((char*)&img.height, 4); // height (4 bytes)
		file.write((char*)&planes, 2); // planes (2 bytes)
		file.write((char*)&bitsPerPixel, 2); // Bit count per pixel (2 bytes)
		file.write(zero, 4); // Compression (4 bytes)
		file.write(zero, 4); // Image size (4 bytes)
		file.write(zero, 4); // X pixels per meter (4 bytes)
		file.write(zero, 4); // Y pixels per meter size (4 bytes)
		file.write(zero, 4); // ColorsUsed (4 bytes)
		file.write(zero, 4); // ColorsImportant (4 bytes)

		for (int y = 0; y < img.height; ++y) {
			for (int x = 0; x < img.width; ++x) {
				int i = y * img.width + x;
				file.write((char*)&img.data[i].b, 1);
				file.write((char*)&img.data[i].g, 1);
				file.write((char*)&img.data[i].r, 1);
			}
			file.write(zero, padding);
		}
	}

	static Image read(const char path[]) {
		ifstream file(path, ios::binary);

		char fileHeaderBuf[14] = {};
		file.read(fileHeaderBuf, 14);

		char signature[3] = { fileHeaderBuf[0], fileHeaderBuf[1], '\0' }; // 2;
		int fileSize = *(int*)(fileHeaderBuf + 2); // 4
		//int reserved = *(int*)(fileHeaderBuf + 6); // 4
		int dataOffset = *(int*)(fileHeaderBuf + 10); // 4

		char infoHeaderBuf[40] = {};
		file.read(infoHeaderBuf, 40);
		int size = *(int*)(infoHeaderBuf); // 4
		int width = *(int*)(infoHeaderBuf + 4); // 4
		int height = *(int*)(infoHeaderBuf + 8); // 4
		//short planes = *(short*)(infoHeaderBuf + 12); // 2
		short bitCount = *(short*)(infoHeaderBuf + 14); // 2
		int compression = *(int*)(infoHeaderBuf + 16); // 4
		//int imageSize = *(int*)(infoHeaderBuf + 20); // 4
		//int xPixelsPerMeter = *(int*)(infoHeaderBuf + 24); // 4
		//int yPixelsPerMeter = *(int*)(infoHeaderBuf + 28); // 4
		//int colorsUsed = *(int*)(infoHeaderBuf + 32); // 4
		//int colorsImportant = *(int*)(infoHeaderBuf + 36); // 4

		if (strcmp(signature, "BM") != 0) { cerr << "Wrong signature"; exit(1); }
		if (bitCount != 24) { cerr << "Only 24-bit colors are supported"; exit(1); }
		if (compression != 0) { cerr << "Compression is not supported"; exit(1); }

		int padding = (((width * 8 * 3) + 31) / 32 * 32 - width * 8 * 3) / 8;
		int rasterDataRowSize = width * 3 + padding;
		int rasterDataSize = rasterDataRowSize * height;
		file.seekg(dataOffset);

		Color24* data = new Color24[width * height];

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				file.read((char*)&data[y * width + x].b, 1);
				file.read((char*)&data[y * width + x].g, 1);
				file.read((char*)&data[y * width + x].r, 1);
			}
			file.ignore(padding);
		}

		return Image(width, height, data);
	}
};

int main() {
	int w, h;
	cout << "Enter width: "; cin >> w;
	cout << "Enter height: "; cin >> h;

	Image image(w, h);

	image.fillTriangle(0, 0, w - 1, 0, w / 2, h - 1, { 255, 255, 255 });

	BMP::write(image, "img.bmp");

	system("img.bmp");

	return 0;
}
