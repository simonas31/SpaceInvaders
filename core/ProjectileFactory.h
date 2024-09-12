#pragma once
#include "../projectiles/Projectile.h"

class ProjectileFactory
{
public:
	static Projectile* createProjectile(const sf::Vector2f& position, SpriteAtlas* atlas, const std::string& projectileName, int frameCount, int direction);
};

