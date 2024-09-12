#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.cpp"
#include <functional>
#include "../core/ProjectileFactory.h"
#include "../core/SpriteAtlas.h"

class Player : public Entity
{
public:
	Player(sf::Vector2f position, SpriteAtlas* atlas);
	void update(float deltaTime);
	void render(sf::RenderWindow& window);
	void shoot(std::function<void(Projectile*)> push_projectile);
	void explode(bool gameOver = false);
	sf::Vector2f getPosition();
};

