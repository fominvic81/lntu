#pragma once

constexpr auto PI = 3.141592653589793;

namespace functions {
	double smoothstep(double t, double t1, double t2);

	double wave(double time, double frequency = 1, double amplitude = 1);
	double wave(double time, double frequency, double min_amp, double max_amp);

	double waveSkewed(double time, double a = 0.5);
	double waveSkewed(double time, double a, double frequency, double amplitude = 1);
	double waveSkewed(double time, double a, double frequency, double min_amp, double max_amp);

	double chirp(double time, double frequency1, double frequency2, double duration, double amplitude = 1);
	double chirp(double time, double frequency1, double frequency2, double duration, double min_amp, double max_amp);

	double chirpLog(double time, double frequency1, double frequency2, double duration, double amplitude = 1, double k = 0);

	double chirpPeriodic(double time, double frequency1, double frequency2, double frequencyOfFrequency, double amplitude = 1);
	double chirpPeriodic(double time, double frequency1, double frequency2, double frequencyOfFrequency, double min_amp, double max_amp);

	/*double chirpSkewed(double time, double a, double frequency1, double frequency2, double duration, double amplitude = 1);
	double chirpSkewed(double time, double a, double frequency1, double frequency2, double duration, double min_amp, double max_amp);

	double chirpPeriodicSkewed(double time, double a, double frequency1, double frequency2, double frequencyOfFrequency, double amplitude = 1);
	double chirpPeriodicSkewed(double time, double a, double frequency1, double frequency2, double frequencyOfFrequency, double min_amp, double max_amp);*/
}