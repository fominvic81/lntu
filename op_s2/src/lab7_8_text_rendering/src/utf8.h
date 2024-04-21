#pragma once
#include <vector>

class UTF8 {
public:
	static std::vector<unsigned int> decode(const char bytes[]);
};