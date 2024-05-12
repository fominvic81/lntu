#include "sound/wave.h"
#include "sound/wav.h"
#include "sound/functions.h"

using namespace std;

int main() {

	Wave wave(20, 0.1);

	wave.sample([](double time) -> double {
		double val = functions::wave(time, 800, 1);

		return val;
	});

	Wav::write(wave, "test.wav");

#ifdef _WIN32
	system("test.wav");
#endif // _WIN32

	return 0;
}