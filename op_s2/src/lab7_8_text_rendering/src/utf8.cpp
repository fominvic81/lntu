#include "utf8.h"

std::vector<unsigned int> UTF8::decode(const char bytes[]) {
	const unsigned char mask1 = 0b11000000;
	const unsigned char mask2 = 0b10000000;

	std::vector<unsigned int> values;

	for (; *bytes; ++bytes) {
		unsigned int value = 0;
		if ((*bytes & mask1) == mask1) {
			if (*(unsigned char*)bytes == 0xff) continue; // Quick inf loop fix
			int count = 2;
			for (; (~*bytes << count & mask2) == 0; ++count);
			unsigned int cb = *bytes & 0xff;
			value |= ((cb << (count + 1) & 0xff) >> (count + 1)) << (6 * (count - 1));
			for (int i = 1; i < count; ++i) {
				cb = bytes[i] & 0xff;
				value |= ((cb << 2 & 0xff) >> 2) << (6 * (count - 1 - i));
			}
			bytes += count - 1;
		} else {
			value = *bytes;
		}
		values.push_back(value);
	}
	
	return values;
}