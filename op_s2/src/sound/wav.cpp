#include <fstream>

#include "wav.h"

void Wav::write(Wave& wave, const char* path) {
	std::ofstream file(path, std::ios::binary);

	int data_size = wave.getDataSize();
	int file_size = 44 + data_size;
	int format_data_size = 16;
	int format_type = 1; // 1 = PCM
	int num_channels = wave.getNumOfChannels();
	int sample_rate = wave.getSampleRate();
	int bits_per_sample = wave.getBitsPerSample();
	int bytes_per_frame = bits_per_sample * num_channels / 8;
	int bytes_per_sec = (sample_rate * bits_per_sample * num_channels) / 8;
	const char* data = wave.getData();

	file.write("RIFF", 4); // RIFF
	file.write((char*)&file_size, 4); // file size
	file.write("WAVE", 4); // File type header
	file.write("fmt ", 4); // Format chunk marker
	file.write((char*)&format_data_size, 4); // Format data size
	file.write((char*)&format_type, 2); // Type of format
	file.write((char*)&num_channels, 2); // Number of channels
	file.write((char*)&sample_rate, 4); // Sample rate
	file.write((char*)&bytes_per_sec, 2); // Bytes per second
	file.write((char*)&bytes_per_frame, 4); // Byts per frame
	file.write((char*)&bits_per_sample, 2); // Bits per sample
	file.write("data", 4); // Data chunk header
	file.write((char*)&data_size, 4); // Size of data
	file.write(data, data_size);
	/*4444422442244
	444442242244*/

}