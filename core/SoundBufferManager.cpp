#include "SoundBufferManager.h"

std::map<std::string, sf::SoundBuffer> SoundBufferManager::buffers;

sf::SoundBuffer& SoundBufferManager::getSoundBuffer(const std::string& filename)
{
    auto it = buffers.find(filename);
    if (it != buffers.end()) {
        return it->second;
    }

    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(filename)) {
        buffers[filename] = buffer;
    }

    return buffers[filename];
}
