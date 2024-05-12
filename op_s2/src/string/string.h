#pragma once
#include <iostream>

class String {
private:
	size_t len;
	char* data;

	String(char text[], size_t text_size);
public:
	String(const char text[]);
	String(const String& str);

	~String();

	String substr(size_t start, size_t n) const;

	size_t size() const;
	const char* c_str() const;

	String& operator=(const String& str);
	String& operator=(const char text[]);

	String operator+(const String& str);
	String operator+(const char text[]);

	char operator[](size_t index);

	operator const char*();
};

std::ostream& operator<<(std::ostream& stream, String& str);
std::ostream& operator<<(std::ostream& stream, String&& str);