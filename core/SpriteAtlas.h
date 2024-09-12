#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class SpriteAtlas {
private:
    sf::Texture* texture;
    std::map<std::string, sf::IntRect> spriteRects;

public:
    SpriteAtlas();
    SpriteAtlas(sf::Texture* tex);

    void addSpriteLocation(const std::string& name, int left, int top, int width, int height);
    sf::Sprite createSprite(const std::string& name);
    sf::IntRect getSpriteRect(const std::string& name);
};
