#include "ProjectileFactory.h"

Projectile* ProjectileFactory::createProjectile(const sf::Vector2f& position, SpriteAtlas* atlas, const std::string& projectileName, int frameCount, int direction)
{
	return new Projectile(position, atlas, projectileName, frameCount, direction);
}
