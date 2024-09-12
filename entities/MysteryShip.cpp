#include "MysteryShip.h"

MysteryShip::MysteryShip(sf::Vector2f position, SpriteAtlas* atlas)
{
	this->atlas = atlas;
	setSprite("mystery_ship");
	sprite.setPosition(position);
	animation = Animation(1.f);
	animation.enableLoop();
	animation.addFrame(atlas->createSprite("mystery_ship").getTextureRect());

	deathAnimation = Animation(.5f);
	deathAnimation.setLoops(1);
	deathAnimation.enableLoop();
	deathAnimation.addFrame(atlas->createSprite("entity_death_1").getTextureRect());
	deathAnimation.addFrame(atlas->createSprite("entity_death_2").getTextureRect());

	movementBuffer.loadFromFile("assets/sounds/mystery_ship_movement.wav");
	deathBuffer.loadFromFile("assets/sounds/alien_explosion.wav");

	movementSound.setBuffer(movementBuffer);
	deathSound.setBuffer(deathBuffer);
	movementSound.setLoop(true);
	movementSound.play();

	lives = 1;
	speed = 100.f;
}

void MysteryShip::update(float deltaTime)
{
	if (!exploding) {
		sf::Vector2f movement(0.f, 0.f);
		if (movingBack) {
			movement.x = -1.f;
		}
		else {
			if (sprite.getPosition().x + 32 >= 450) {
				movingBack = true;
			}
			movement.x = 1.f;
		}

		sprite.move(movement * speed * deltaTime);
		animation.update(deltaTime, sprite);
	}
	else {
		if (playDeathSoundOnce) {
			deathSound.play();
			playDeathSoundOnce = false;
		}
		//deathSound.pause();
		deathAnimation.update(deltaTime, sprite, true);

		if (deathAnimation.isFinished()) {
			exploding = false;
		}
	}
}

void MysteryShip::render(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void MysteryShip::shoot(std::function<void(Projectile*)> push_projectile)
{
}

void MysteryShip::explode()
{
	lives--;
	exploding = true;
}
