#include "Projectile.h"

Projectile::Projectile(sf::Vector2f position, SpriteAtlas* atlas, const std::string& projectileName, int frameCount, int dir)
{
	explosionAnimation = Animation(.25f);
	explosionAnimation.enableLoop();
	explosionAnimation.setLoops(1);
	explosionAnimation.addFrame(atlas->createSprite("projectile_explosion_1").getTextureRect());
	explosionAnimation.addFrame(atlas->createSprite("projectile_explosion_2").getTextureRect());

	sprite = atlas->createSprite(projectileName + "_1");
	projectileAnimation = Animation(.25f);
	sprite.setScale(sf::Vector2f(2.f, 2.f));
	sprite.setPosition(position);

	for (int i = 1; i <= frameCount; i++) {
		projectileAnimation.addFrame(atlas->createSprite(projectileName + "_" + std::to_string(i)).getTextureRect());
	}

	speed = 150.f;
	direction = dir;
}

void Projectile::update(float deltaTime)
{
	if (!exploding) {
		// Move the projectile in the direction
		sprite.move(0.f, speed * deltaTime * direction);
		projectileAnimation.update(deltaTime, sprite);
	}
	else {
		// Handle explosion timing
		explosionAnimation.update(deltaTime, sprite, true);

		// If the explosion animation has finished, mark the projectile as "completely exploded"
		if (explosionAnimation.isFinished()) {
			exploded = true;
		}
	}
}

void Projectile::render(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Projectile::explode(bool forceUpdateAnimation)
{
	sf::Vector2f pos = sprite.getPosition();
	// force update animation so when hit obstacle explosion will be seen first
	if (forceUpdateAnimation) {
		pos.y += 4.f * direction;
	}
	pos.x -= 4.f;
	sprite.setPosition(pos);


	exploding = true;
}

bool Projectile::isExploded() const
{
	return exploded;
}

bool Projectile::isExploding() const
{
	return exploding;
}

bool Projectile::isOffBoundaries(float yBoundry)
{
	return sprite.getPosition().y >= yBoundry - 30.f || sprite.getPosition().y <= 30.f;
}

void Projectile::setSpeed(float newSpeed)
{
	this->speed = newSpeed;
}

sf::FloatRect Projectile::getBounds()
{
	return sprite.getGlobalBounds();
}

sf::Vector2f Projectile::getPosition()
{
	return sprite.getPosition();
}

sf::Sprite Projectile::getSprite()
{
	return sprite;
}

int Projectile::getDirection()
{
	return direction;
}
