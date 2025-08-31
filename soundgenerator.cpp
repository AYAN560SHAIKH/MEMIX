#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdint>

const int sampleRate = 44100;
const int durationSeconds = 1;
const int amplitude = 28000;

void write_wav(const std::string& filename, double frequency) {
    std::ofstream file(filename, std::ios::binary);

    int dataSize = sampleRate * durationSeconds * 2; // 2 bytes per sample
    int fileSize = 36 + dataSize;

    // WAV Header
    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char*>(&fileSize), 4);
    file.write("WAVE", 4);

    file.write("fmt ", 4);
    int fmtChunkSize = 16;
    short audioFormat = 1; // PCM
    short numChannels = 1;
    int byteRate = sampleRate * numChannels * 2;
    short blockAlign = numChannels * 2;
    short bitsPerSample = 16;

    file.write(reinterpret_cast<const char*>(&fmtChunkSize), 4);
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    file.write(reinterpret_cast<const char*>(&sampleRate), 4);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    // Data Chunk
    file.write("data", 4);
    file.write(reinterpret_cast<const char*>(&dataSize), 4);

    for (int i = 0; i < sampleRate * durationSeconds; ++i) {
        double t = static_cast<double>(i) / sampleRate;
        short sample = static_cast<short>(amplitude * sin(2 * M_PI * frequency * t));
        file.write(reinterpret_cast<const char*>(&sample), 2);
    }

    file.close();
    std::cout << "Generated: " << filename << std::endl;
}

int main() {
    write_wav("flip.wav",     440.0);  // A4
    write_wav("match.wav",    660.0);  // E5
    write_wav("nomatch.wav",  330.0);  // E4
    write_wav("timeout.wav",  200.0);  // Low buzz
    write_wav("bgmusic.wav",  523.3);  // C5 tone (can be looped)

    std::cout << "All sound effects generated successfully.\n";
    return 0;
}
