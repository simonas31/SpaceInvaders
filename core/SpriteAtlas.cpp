#include "SpriteAtlas.h"

SpriteAtlas::SpriteAtlas()
{
	texture = nullptr;
}

SpriteAtlas::SpriteAtlas(sf::Texture* tex) : texture(tex)
{
}

void SpriteAtlas::addSpriteLocation(const std::string& name, int left, int top, int width, int height) {
	spriteRects[name] = sf::IntRect(left, top, width, height);
}

sf::Sprite SpriteAtlas::createSprite(const std::string& name) {
	sf::Sprite sprite(*texture);
	sprite.setTextureRect(spriteRects[name]);
	return sprite;
}

sf::IntRect SpriteAtlas::getSpriteRect(const std::string& name)
{
	return spriteRects[name];
}
