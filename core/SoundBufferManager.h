#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class SoundBufferManager
{
public:
    static sf::SoundBuffer& getSoundBuffer(const std::string& filename);

private:
    static std::map<std::string, sf::SoundBuffer> buffers;
};

