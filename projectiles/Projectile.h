#pragma once
#include "SFML/Graphics.hpp"
#include "../animations/Animation.h"
#include "../core/SpriteAtlas.h"

class Projectile
{
private:
	Animation projectileAnimation;
	Animation explosionAnimation;
	sf::Sprite sprite;
	float speed;
	int direction;
	bool exploded = false, exploding = false;  // New flag to track if projectile exploded

public:
	Projectile(sf::Vector2f position, SpriteAtlas* atlas, const std::string& projectileName, int frameCount, int dir);
	void update(float deltaTime);
	void render(sf::RenderWindow& window);
	void explode(bool forceUpdateAnimation = false);
	bool isExploded() const;
	bool isExploding() const;
	bool isOffBoundaries(float yBoundry);
	void setSpeed(float newSpeed);
	sf::FloatRect getBounds();
	sf::Vector2f getPosition();
	sf::Sprite getSprite();
	int getDirection();
};

