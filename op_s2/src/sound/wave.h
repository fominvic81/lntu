#pragma once
#include <vector>
#include <bitset>
#include <stdint.h>

class Wave {
	const int bytes_per_sample = 2;
	size_t samples_per_channel;
	int sample_rate;
	int channels;
	double max_amplitude;
	int16_t* data;
public:

	Wave(double duration, double max_amplitude = 0.1, int sample_rate = 48000, int channels = 2/*, int bytes_per_sample = 2*/);

	size_t getDataSize() const;
	double getDuration() const;
	int getNumOfChannels() const;
	int getSampleRate() const;
	int getBitsPerSample() const;
	const char* getData() const;

	void setSample(int index, int channel, int16_t value);

	void noisify(double amplitude);
	void sample(double(*pred)(double time));
};