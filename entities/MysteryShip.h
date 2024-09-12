#pragma once
#include "Entity.cpp"

class MysteryShip :
    public Entity
{
private:
	bool movingBack = false;
public:
	MysteryShip(sf::Vector2f position, SpriteAtlas* atlas);
	void update(float deltaTime);
	void render(sf::RenderWindow& window);
	void shoot(std::function<void(Projectile*)> push_projectile);
	void explode();
};

