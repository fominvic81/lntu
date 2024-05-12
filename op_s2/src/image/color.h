#pragma once
#include <cstdint>

class Color24 {
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;

	Color24();
	Color24(int r, int g, int b);
};

class Color {
public:
	int r;
	int g;
	int b;

	Color();
	Color(int r, int g, int b);
	Color(Color24 color);

	Color operator+(Color& rls) const;
	Color operator+(Color rls) const;
	Color operator-(Color& rls) const;
	Color operator-(Color rls) const;
	Color operator*(int rls) const;
	Color operator*(Color rls) const;
	Color operator/(int rls) const;
};