#include <cmath>

#include "functions.h"

double functions::smoothstep(double t, double t1, double t2) {
	if (t1 > t2) return 1 - smoothstep(t, t2, t1);
	if (t < t1) return 0;
	if (t > t2) return 1;
	return 3 * pow((t - t1) / (t2 - t1), 2) - 2 * pow((t - t1) / (t2 - t1), 3);
}

#define min_max_amp(x) (x + 1) / 2 * (max_amp - min_amp) + min_amp

double functions::wave(double time, double frequency, double amplitude) {
	return (sin(2 * PI * time * frequency)) * amplitude;
}

double functions::wave(double time, double frequency, double min_amp, double max_amp) {
	return min_max_amp(sin(2 * PI * time * frequency));
}

double functions::waveSkewed(double time, double a) {
	double x = fmod(time, 1);
	return x < a ? -cos(x * PI / a) : cos((x - a) * PI / (1 - a));
}

double functions::waveSkewed(double time, double a, double frequency, double amplitude) {
	return functions::waveSkewed(time * frequency, a) * amplitude;
}

double functions::waveSkewed(double time, double a, double frequency, double min_amp, double max_amp) {
	return min_max_amp(functions::waveSkewed(time * frequency, a));
}

double functions::chirp(double time, double frequency1, double frequency2, double duration, double amplitude) {
	if (time < 0 || time >= duration) return 0;
	return sin(2 * PI * (time * frequency1 + time * time * (frequency2 - frequency1) / 2 / duration)) * amplitude;
}

double functions::chirp(double time, double frequency1, double frequency2, double duration, double min_amp, double max_amp) {
	if (time < 0 || time >= duration) return 0;
	return min_max_amp(sin(2 * PI * (time * frequency1 + time * time * (frequency2 - frequency1) / 2 / duration)));
}

// https://www.desmos.com/calculator/knrporqjxk
double functions::chirpLog(double time, double frequency1, double frequency2, double duration, double amplitude, double k) {
	if (time < 0 || time >= duration) return 0;
	if (k == 0) k = (frequency1 < frequency2 ? 1 : -1) * duration;
	double x = time;
	double a = frequency1;
	double b = frequency2;
	double c = duration;
	if (k > 0) {
		return sin(2 * PI * ((a - b) * ((k * x - c * k - 1) * log(c * k - k * x + 1) - k * x) / (k * log(c * k + 1)) + b * x)) * amplitude;
	} else {
		k *= -1;
		return sin(2 * PI * (a * x + (b - a) * ((k * x + 1) * log(k * x + 1) - k * x) / (k * log(c * k + 1)))) * amplitude;
	}
}

double functions::chirpPeriodic(double time, double frequency1, double frequency2, double frequencyOfFrequency, double amplitude) {
	time -= 0.5 / frequencyOfFrequency;
	double t = (time + sin(2 * PI * frequencyOfFrequency * time) / 2 / PI / frequencyOfFrequency) / 2;
	return sin(2 * PI * (frequency1 * time + (frequency2 - frequency1) * t)) * amplitude;
}

double functions::chirpPeriodic(double time, double frequency1, double frequency2, double frequencyOfFrequency, double min_amp, double max_amp) {
	time -= 0.5 / frequencyOfFrequency;
	double t = (time + sin(2 * PI * frequencyOfFrequency * time) / 2 / PI / frequencyOfFrequency) / 2;
	return min_max_amp(sin(2 * PI * (frequency1 * time + (frequency2 - frequency1) * t)));
}