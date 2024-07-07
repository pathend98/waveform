#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>

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

std::shared_ptr<std::vector<float>> normaliseAudioData(const std::vector<int16_t>& data)
{

    std::shared_ptr<std::vector<float>> normalisedData = std::make_shared<std::vector<float>>();
    int16_t maxSample = *std::max_element(data.begin(), data.end());
    int16_t minSample = *std::min_element(data.begin(), data.end());
    int16_t maxAbsSample = std::max(std::abs(maxSample), std::abs(minSample));

    for (int16_t sample : data)
        normalisedData->push_back(static_cast<float>(sample) / maxAbsSample);

    return normalisedData;
}

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

    // std::cout << "Chunk Size: " << header.chunnkSize << std::endl;
    // std::cout << "Sub-Chunk 1 Size: " << header.subChunk1Size << std::endl;
    // std::cout << "Audio Format: " << header.audioFormat << std::endl;
    // std::cout << "Number of Channels: " << header.numChannels << std::endl;
    // std::cout << "Sample Rate: " << header.sampleRate << std::endl;
    // std::cout << "Byte Rate: " << header.byteRate << std::endl;
    // std::cout << "Block Align: " << header.blockAlign << std::endl;
    // std::cout << "Bits per Sample: " << header.bitsPerSample << std::endl; 
    // std::cout << "Sub-Chunk 2 Size: " << header.subChunk2Size << std::endl;

    // Bits per sample is 16 here, but probably shouldn't use int16_t
    std::vector<int16_t> audioData(header.subChunk2Size / sizeof(int16_t));
    file.read(reinterpret_cast<char *>(audioData.data()), header.subChunk2Size);
    auto normalised = normaliseAudioData(audioData);

    file.close();
}

int main()
{
    readWAV("files/StarWars3.wav");
    return 0;
}
