#include <iostream>
#include "image/image.h"
#include "image/bmp.h"

using namespace std;

int main() {
	int w, h;
	cout << "Enter width: "; cin >> w;
	cout << "Enter height: "; cin >> h;

	Image image(w, h);

	image.fillTriangle(0, h - 1, w - 1, h - 1, w / 2, 0, { 255, 255, 255 });

	BMP::write(image, "img.bmp");

#ifdef _WIN32
	system("img.bmp");
#endif // _WIN32

	return 0;
}
