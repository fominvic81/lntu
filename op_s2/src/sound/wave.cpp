#include <numeric>
#include <cmath>
#include <random>
#include <limits>

#include "wave.h"

Wave::Wave(double duration, double max_amplitude, int sample_rate, int channels/*, int bytes_per_sample*/) {
	this->samples_per_channel = ceil(duration * sample_rate);
	this->sample_rate = sample_rate;
	this->channels = channels;
	this->max_amplitude = std::max(0.0, std::min(1.0, max_amplitude));
	//this->bytes_per_sample = bytes_per_sample;
	this->data = new int16_t[getDataSize() / sizeof(*data)];
}

size_t Wave::getDataSize() const {
	return (size_t)channels * bytes_per_sample * samples_per_channel;
}
double Wave::getDuration() const {
	return samples_per_channel / sample_rate;
}

int Wave::getNumOfChannels() const {
	return channels;
}
int Wave::getSampleRate() const {
	return sample_rate;
}
int Wave::getBitsPerSample() const {
	return bytes_per_sample * 8;
}
const char* Wave::getData() const {
	return (char*)data;
}

void Wave::setSample(int index, int channel, int16_t value) {
	data[index * channels + channel] = value * max_amplitude;
}

void Wave::noisify(double amplitude) {

	std::mt19937 gen(123);
	std::uniform_int_distribution<int16_t> dist(std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max());

	for (size_t i = 0; i < samples_per_channel; ++i) {
		int16_t val = dist(gen) * amplitude;

		for (int channel = 0; channel < channels; ++channel) {
			setSample(i, channel, val);
		}
	}
}

void Wave::sample(double(*pred)(double time)) {
	for (size_t i = 0; i < samples_per_channel; ++i) {
		double normalized = std::max(-1.0, std::min(1.0, pred((double)i / sample_rate)));
		int16_t value = normalized * (normalized < 0 ?
				-std::numeric_limits<int16_t>::min() :
				 std::numeric_limits<int16_t>::max());

		for (int channel = 0; channel < channels; ++channel) {
			setSample(i, channel, value);
		}
	}
}