#include <iostream>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif // _WIN32

#include "color.h"
#include "image.h"
#include "bmp.h"
#include "utf8.h"
#include "string.h"

#define degToRad 3.141592653589793f / 180.0f

using namespace std;

#include "../../../fonts/ubuntu.mono.ttf.inc"
Font font(loadFontStatic(ubuntu_mono));
//Font font("fonts/ubuntu.mono.ttf");

int main(int argc, char** argv) {
	int w, h;
	w = 1000; h = 180;

	Image image(w, h);
	image.fill(Color(25, 25, 25));
	image.drawText(font, 0, 50, (char*)u8"Привіт світ", 50, { 200, 200, 200 }, 0, Center);

	BMP::write(image, "img.bmp");
	system("img.bmp");
	return 0;
}
