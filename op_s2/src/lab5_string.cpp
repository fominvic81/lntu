#include <iostream>
#include <cstring>

using namespace std;

class String {
private:
	size_t len;
	char* data;

	String(char text[], size_t text_size) {
		data = text;
		len = text_size;
	}

public:
	String(const char text[]) {
		len = strlen(text);
		data = new char[len + 1];

		memcpy(data, text, len);
		data[len] = '\0';
	}

	String(const String& str) {
		len = str.len;
		data = new char[len + 1];

		memcpy(data, str.data, len);
		data[len] = '\0';
	}

	~String() {
		delete[] data;
	}

	String substr(size_t start, size_t n) {
		char* arr = new char[n + 1];
		memcpy(arr, data + start, n);
		arr[n] = '\0';
		return String(arr, n);
	}

	size_t size() const {
		return len;
	}

	const char* c_str() const {
		return this->data;
	}

	String& operator=(const String& str) {
		if (this == &str) return *this;
		delete[] data;
		len = str.len;
		data = new char[len + 1];

		memcpy(data, str.data, len);
		data[len] = '\0';

		return *this;
	}
	String& operator=(const char text[]) {
		delete[] data;
		len = strlen(text);
		data = new char[len + 1];

		memcpy(data, text, len);
		data[len] = '\0';

		return *this;
	}

	String operator+(const String& str) {
		size_t str_size = str.len;
		char* new_text = new char[len + str_size + 1];

		memcpy(new_text, data, len);
		memcpy(new_text + len, str.data, str.len);
		new_text[len + str_size] = '\0';

		return String(new_text, str_size + len);
	}

	String operator+(const char text[]) {
		size_t text_size = strlen(text);
		char* new_text = new char[len + text_size + 1];

		memcpy(new_text, data, len);
		memcpy(new_text + len, text, text_size);
		new_text[len + text_size] = '\0';

		return String(new_text, text_size + len);
	}

	char operator[](size_t index) {
		return data[index];
	}
};

ostream& operator<<(ostream& stream, String& str) {
	stream.write(str.c_str(), str.size());
	return stream;
}
ostream& operator<<(ostream& stream, String&& str) {
	stream.write(str.c_str(), str.size());
	return stream;
}

int main() {

	String hello = "Hello";
	String world = "world";

	String hw = hello + " " + world;

	cout << hw << " " << hw.size() << endl;
	cout << hw[1] << endl;

	return 0;
}
