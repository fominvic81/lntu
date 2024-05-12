#include <fstream>
#include <iostream>

#include "bmp.h"

const int BMP::header_size = 14;
const int BMP::info_header_size= 40;

void BMP::write(const Image& img, const char path[]) {
	std::ofstream file(path, std::ios::binary);

	int padding = (((img.width * 8 * 3) + 31) / 32 * 32 - img.width * 8 * 3) / 8;
	int raster_data_size = (img.width * 3 + padding) * img.height;
	int file_size = header_size + info_header_size + raster_data_size;
	int dataOffset = header_size + info_header_size;
	int planes = 1;
	int bits_per_pixel = 8 * 3;

	const char* zero = "\0\0\0\0";

	// Header
	file.write("BM", 2); // Signature (2 bytes)
	file.write((char*)&file_size, 4); // File size (4 bytes)
	file.write(zero, 4); // Reserved (4 bytes)
	file.write((char*)&dataOffset, 4); // Data offset (4 bytes)
	// Info header
	file.write((char*)&info_header_size, 4); // Info header size (4 bytes)
	file.write((char*)&img.width, 4); // width (4 bytes)
	file.write((char*)&img.height, 4); // height (4 bytes)
	file.write((char*)&planes, 2); // planes (2 bytes)
	file.write((char*)&bits_per_pixel, 2); // Bit count per pixel (2 bytes)
	file.write(zero, 4); // Compression (4 bytes)
	file.write(zero, 4); // Image size (4 bytes)
	file.write(zero, 4); // X pixels per meter (4 bytes)
	file.write(zero, 4); // Y pixels per meter size (4 bytes)
	file.write(zero, 4); // ColorsUsed (4 bytes)
	file.write(zero, 4); // ColorsImportant (4 bytes)

	for (int y = img.height - 1; y >= 0; --y) {
		for (int x = 0; x < img.width; ++x) {
			int i = y * img.width + x;
			file.write((char*)&img.data[i].b, 1);
			file.write((char*)&img.data[i].g, 1);
			file.write((char*)&img.data[i].r, 1);
		}
		file.write(zero, padding);
	}
}

Image BMP::read(const char path[]) {
	std::ifstream file(path, std::ios::binary);

	char file_header_buf[14] = {};
	file.read(file_header_buf, 14);

	char signature[3] = { file_header_buf[0], file_header_buf[1], '\0' }; // 2;
	//int fileSize = *(int*)(file_header_buf + 2); // 4
	//int reserved = *(int*)(fileHeaderBuf + 6); // 4
	int data_offset = *(int*)(file_header_buf + 10); // 4

	char info_header_buf[40] = {};
	file.read(info_header_buf, 40);
	//int size = *(int*)(info_header_buf); // 4
	int width = *(int*)(info_header_buf + 4); // 4
	int height = *(int*)(info_header_buf + 8); // 4
	//short planes = *(short*)(info_header_buf + 12); // 2
	short bit_count = *(short*)(info_header_buf + 14); // 2
	int compression = *(int*)(info_header_buf + 16); // 4
	//int image_size = *(int*)(info_header_buf + 20); // 4
	//int x_pixels_per_meter = *(int*)(info_header_buf + 24); // 4
	//int y_pixels_per_meter = *(int*)(info_header_buf + 28); // 4
	//int colors_used = *(int*)(info_header_buf + 32); // 4
	//int colors_important = *(int*)(info_header_buf + 36); // 4

	if (strcmp(signature, "BM") != 0) { std::cerr << "Wrong signature"; exit(1); }
	if (bit_count != 24) { std::cerr << "Only 24-bit colors are supported"; exit(1); }
	if (compression != 0) { std::cerr << "Compression is not supported"; exit(1); }

	int padding = (((width * 8 * 3) + 31) / 32 * 32 - width * 8 * 3) / 8;
	//int raster_data_row_size = width * 3 + padding;
	//int raster_data_size = raster_data_row_size * height;
	file.seekg(data_offset);

	if (width * height * sizeof(Color24) > 256 * 1000 * 1000) {
		// TODO
	}

	Color24* data = new Color24[width * height];

	for (int y = height - 1; y >= 0; --y) {
		for (int x = 0; x < width; ++x) {
			file.read((char*)&data[y * width + x].b, 1);
			file.read((char*)&data[y * width + x].g, 1);
			file.read((char*)&data[y * width + x].r, 1);
		}
		file.ignore(padding);
	}

	return Image(width, height, data);
}