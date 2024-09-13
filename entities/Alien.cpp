#include "Alien.h"

Alien::Alien(const struct AlienType& alienType, sf::Vector2f pos, SpriteAtlas* atlas, int column)
{
	this->atlas = atlas;
	alien_name = alienType.alienName;
	alienScore = alienType.alienScore;
	setSprite(alien_name + "_sprite_1");

	// Base animation
	animation = Animation(4.f);
	animation.addFrame(atlas->createSprite(alien_name + "_sprite_1").getTextureRect());
	animation.addFrame(atlas->createSprite(alien_name + "_sprite_2").getTextureRect());

	// Death animation
	deathAnimation = Animation(.5f);
	deathAnimation.setLoops(1);
	deathAnimation.enableLoop();
	deathAnimation.addFrame(atlas->createSprite("entity_death_1").getTextureRect());
	deathAnimation.addFrame(atlas->createSprite("entity_death_2").getTextureRect());

	shootSound.setBuffer(SoundBufferManager::getSoundBuffer("assets/sounds/shoot.wav"));
	deathSound.setBuffer(SoundBufferManager::getSoundBuffer("assets/sounds/alien_explosion.wav"));

	sprite.setPosition(pos);
	speed = 100.f;
	lives = 1;
	this->column = column;
}

void Alien::update(float deltaTime)
{
	if (!exploding) {
		shootTimer += deltaTime;
		movementTimer += deltaTime;

		// animation based movement
		if (movementTimer >= FRAME_TIME / speedFactor) {
			animation.update(4.f, sprite, true);

			sf::Vector2f movement(0.f, 0.f);
			// if changing direction then go down
			if (changeDirection) {
				changeDirection = false;
				movement.y = stepSize * 1.5f;
			}
			else {
				movement.x = direction * stepSize;
			}
			sprite.move(movement);
			movementTimer = 0.f;// reset movement timer
		}
	}
	else {
		if (playDeathSoundOnce) {
			deathSound.play();
			playDeathSoundOnce = false;
		}
		deathAnimation.update(deltaTime, sprite, true);

		if (deathAnimation.isFinished()) {
			exploding = false;
		}
	}
}

void Alien::render(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Alien::shoot(std::function<void(Projectile*)> push_projectile)
{
	if (canShoot() && !exploding) {
		shootSound.play();
		sf::Vector2f pos = sprite.getPosition() + (sprite.getGlobalBounds().getSize() / 2.f);
		pos.x -= 3.f;
		push_projectile(ProjectileFactory::createProjectile(pos, atlas, alien_name + "_projectile", 4, 1));
		shootTimer = 0.f;  // Reset timer after shooting
	}
}

void Alien::explode()
{
	lives--;
	exploding = true;
}

int Alien::getScore() const
{
	return alienScore;
}

void Alien::setSpeedFactor(float newSpeedFactor)
{
	speedFactor = newSpeedFactor;
}

bool Alien::isOffBoundaries(float xBoundry)
{
	// if going right check right boundary
	if (direction > 0) {
		return sprite.getPosition().x + 24.f >= xBoundry;
	}
	// if going left check left boundary

	return sprite.getPosition().x <= 10.f;
}

void Alien::reverseDirectrion()
{
	if (!changeDirection) { //change direction if previously was not changed
		movementTimer = 0.f;
		changeDirection = true;
		direction *= -1;
	}
}

sf::Vector2f Alien::getPosition()
{
	return sprite.getPosition();
}

int Alien::getColumn() const
{
	return column;
}
