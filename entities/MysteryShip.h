#pragma once
#include "Entity.cpp"
#include "../core/SoundBufferManager.h"

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
	void stopSounds();
};

