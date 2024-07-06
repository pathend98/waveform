#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

using std::string;

// The spec for WAVE files: http://soundfile.sapp.org/doc/WaveFormat/
struct WAVEHeader 
{

    // RIFF Chunk Descriptor
    char riff[4];
    uint32_t chunnkSize;
    char wave[4];

    // fmt sub-chunk
    char fmt[4];
    uint32_t subChunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;

    // data sub-chunk
    char data[4];
    uint32_t subChunk2Size;
};

void readWAV(const string filename) 
{
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    WAVEHeader header;
    file.read(reinterpret_cast<char *>(&header), sizeof(WAVEHeader));

    if (strncmp(header.riff, "RIFF", 4) != 0 ||
        strncmp(header.wave, "WAVE", 4) != 0 ||
        strncmp(header.fmt, "fmt ", 4) != 0 ||
        strncmp(header.data, "data", 4) != 0)
    {
        std::cerr << "Invalid WAV file format" << std::endl;
        file.close();
        return;
    }

    std::cout << "Sub-Chunk 1 Size: " << header.subChunk1Size << std::endl;
    std::cout << "Audio Format: " << header.audioFormat << std::endl;
    std::cout << "Number of Channels: " << header.numChannels << std::endl;
    std::cout << "Sample Rate: " << header.sampleRate << std::endl;
    std::cout << "Byte Rate: " << header.byteRate << std::endl;
    std::cout << "Block Align: " << header.blockAlign << std::endl;
    std::cout << "Bits per Sample: " << header.bitsPerSample << std::endl;
    std::cout << "Sub-Chunk 2 Size: " << header.subChunk2Size << std::endl;

    file.close();
}

int main()
{
    readWAV("files/StarWars3.wav");
    return 0;
}
