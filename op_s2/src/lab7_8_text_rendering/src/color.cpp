#include "color.h"

Color24::Color24() : r(0), g(0), b(0) {}
Color24::Color24(int r, int g, int b) : r(r), g(g), b(b) {}

Color::Color() : r(0), g(0), b(0) {}
Color::Color(int r, int g, int b) : r(r), g(g), b(b) {}
Color::Color(Color24 color) : r(color.r), g(color.g), b(color.b) {}

Color Color::operator+(Color& rls) const { return { r + rls.r, g + rls.g, b + rls.b }; }
Color Color::operator+(Color rls) const { return { r + rls.r, g + rls.g, b + rls.b }; }
Color Color::operator-(Color& rls) const { return { r - rls.r, g - rls.g, b - rls.b }; }
Color Color::operator-(Color rls) const { return { r - rls.r, g - rls.g, b - rls.b }; }
Color Color::operator*(int rls) const { return { r * rls, g * rls, b * rls }; }
Color Color::operator*(Color rls) const { return { r * rls.r, g * rls.g, b * rls.b }; }
Color Color::operator/(int rls) const { return { r / rls, g / rls, b / rls }; }