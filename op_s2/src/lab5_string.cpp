#include <iostream>
#include "string/string.h"

using namespace std;

int main() {

	String hello = "Hello";
	String world = "world";

	String hw = hello + " " + world;

	cout << hw << " " << hw.size() << endl;
	cout << hw[1] << endl;

	return 0;
}
