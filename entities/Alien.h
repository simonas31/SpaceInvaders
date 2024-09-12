#pragma once
#include "Entity.cpp"
#include <functional>
#include "../core/ProjectileFactory.h"
#include "../core/AlienType.cpp"

#define FRAME_TIME .5f

class Alien :
    public Entity
{
private:
	std::string alien_name;
	int alienScore, column;
	float stepSize = 8.f, direction = 1.f, movementTimer = 0.f, speedFactor = .5f; // max speed factor 6
	bool changeDirection = false, recentlyChangedDirection = false;
public:
	Alien(const struct AlienType& alienType, sf::Vector2f pos, SpriteAtlas* atlas, int column);
	void update(float deltaTime);
	void render(sf::RenderWindow& window);
	void shoot(std::function<void(Projectile*)> push_projectile);
	void explode();
	int getScore() const;
	void setSpeedFactor(float newSpeedFactor);
	bool isOffBoundaries(float xBoundry);
	void reverseDirectrion();
	sf::Vector2f getPosition();
	int getColumn() const;
};

