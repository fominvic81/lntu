#include <iostream>
#include "image/color.h"
#include "image/image.h"
#include "image/bmp.h"
#include "text/utf8.h"
#include "string/string.h"

using namespace std;

#include "../fonts/ubuntu.mono.ttf.inc"
Font font(loadFontStatic(ubuntu_mono));

int main() {
	int w, h;
	w = 60; h = 60;

	Image image(w, h);
	image.fill(Color(25, 25, 25));
	image.drawText(font, 0, 0, (char*)u8"Ф", 50, { 200, 200, 200 }, 0, Left);

	BMP::write(image, "img.bmp");
	
#ifdef _WIN32
		system("img.bmp");
#endif // _WIN32

	return 0;
}
