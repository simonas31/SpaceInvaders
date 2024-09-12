#include "Wall.h"

Wall::Wall(sf::Vector2f position, SpriteAtlas* atlas)
{
	this->atlas = atlas;
	sprite = atlas->createSprite("wall");
	sprite.setScale(sf::Vector2f(2.f, 2.f));

	position.x += 30.f;
	this->position = position;

	sprite.setPosition(this->position);
}

void Wall::update(float deltaTime)
{

}

void Wall::render(sf::RenderWindow& window)
{
	window.draw(sprite);
}

int generateRandomInRange(int left, int right, int value) {
	int lowerBound = std::max(left, value - 3);
	int upperBound = std::min(right, value + 3);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(lowerBound, upperBound);

	return dist(gen);
}

void Wall::destroy(sf::FloatRect bounds)
{
	sf::IntRect collision = getSpriteCollision(bounds);
	sf::Image img = sprite.getTexture()->copyToImage();

	sf::IntRect wallBoundries = sprite.getTextureRect();
	for (int i = collision.top; i < collision.top + collision.height; i++) {
		for (int j = collision.left; j < collision.left + collision.width; j++) {
			//destroy where was hit
			if (img.getPixel(j, i) != sf::Color::Transparent) {
				img.setPixel(j, i, sf::Color::Transparent);
			}

			//destroy randomly around current pixel
			for (int r = 0; r < rand() % 5 + 1; r++)
				img.setPixel(generateRandomInRange(wallBoundries.left, wallBoundries.left + wallBoundries.width, j), i, sf::Color::Transparent);
		}
	}

	sf::Texture* texture = new sf::Texture();
	texture->loadFromImage(img);
	sprite.setTexture(*texture);
}

bool Wall::intersects(sf::FloatRect bounds)
{
	if (!sprite.getGlobalBounds().intersects(bounds))
		return false;

	sf::Image img = sprite.getTexture()->copyToImage();
	sf::IntRect collision = getSpriteCollision(bounds);

	//check only that part of bound that are inside wall
	for (int i = collision.top; i < collision.top + collision.height; i++) {
		for (int j = collision.left; j < collision.left + collision.width; j++) {
			if (img.getPixel(j, i) != sf::Color::Transparent) {
				return true;
			}
		}
	}

	return false;
}

sf::IntRect Wall::getSpriteCollision(sf::FloatRect bounds)
{
	sf::IntRect spriteTextureRect = sprite.getTextureRect();
	sf::FloatRect spriteBounds = sprite.getGlobalBounds();
	sf::Vector2f spriteScale = sprite.getScale();
	int startX = std::max(spriteTextureRect.left, static_cast<int>(spriteTextureRect.left + roundf((bounds.left - spriteBounds.left) / spriteScale.x)));
	int endX = std::min(spriteTextureRect.left + spriteTextureRect.width - 1, static_cast<int>(spriteTextureRect.left + roundf((bounds.left + bounds.width - spriteBounds.left) / spriteScale.x)));
	int startY = std::max(spriteTextureRect.top, static_cast<int>(spriteTextureRect.top + roundf((bounds.top - spriteBounds.top) / spriteScale.y)));
	int endY = std::min(spriteTextureRect.top + spriteTextureRect.height - 1, static_cast<int>(spriteTextureRect.top + roundf((bounds.top + bounds.height - spriteBounds.top) / spriteScale.y)));
	return sf::IntRect(startX, startY, abs(endX - startX) + 1, abs(endY - startY) + 1);
}

sf::FloatRect Wall::getBounds() const
{
	return sf::FloatRect();
}