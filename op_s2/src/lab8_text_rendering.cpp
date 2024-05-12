#include <iostream>
#include "image/color.h"
#include "image/image.h"
#include "image/bmp.h"
#include "text/utf8.h"
#include "string/string.h"

using namespace std;

#include "../fonts/ubuntu.mono.ttf.inc"
Font font(loadFontStatic(ubuntu_mono));
//Font font("fonts/ubuntu.mono.ttf");

int main() {
	int w, h;
	w = 1000; h = 180;

	Image image(w, h);
	image.fill(Color(25, 25, 25));
	image.drawText(font, 0, 50, (char*)u8"Привіт світ", 50, { 200, 200, 200 }, 0, Center);

	BMP::write(image, "img.bmp");

#ifdef _WIN32
	system("img.bmp");
#endif // _WIN32

	return 0;
}
