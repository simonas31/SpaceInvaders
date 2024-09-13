#include "Player.h"

Player::Player(sf::Vector2f position, SpriteAtlas* atlas)
{
	this->atlas = atlas;
	setSprite("player");
	sprite.setPosition(position);
	animation = Animation(1.f);
	animation.enableLoop();
	animation.addFrame(atlas->createSprite("player").getTextureRect());

	deathAnimation = Animation(2.f);
	deathAnimation.enableLoop();
	deathAnimation.addFrame(atlas->createSprite("entity_death_1").getTextureRect());
	deathAnimation.addFrame(atlas->createSprite("entity_death_2").getTextureRect());

	shootSound.setBuffer(SoundBufferManager::getSoundBuffer("assets/sounds/shoot.wav"));
	deathSound.setBuffer(SoundBufferManager::getSoundBuffer("assets/sounds/player_explosion.wav"));

	shootCooldown = .5f;
	shootTimer = .5f;

	speed = 200.f;
	lives = 3;
}

void Player::update(float deltaTime)
{
	if (!exploding && getLives() > 0) {
		sf::Vector2f movement(0.f, 0.f);

		// Control player movement with keyboard
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			movement.x -= 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			movement.x += 1.f;
		}
		// Update shoot timer
		shootTimer += deltaTime;

		animation.update(deltaTime, sprite, true);
		sprite.move(movement * speed * deltaTime);
	}
	else if(exploding) {
		if (playDeathSoundOnce) {
			deathSound.play();
			playDeathSoundOnce = false;
		}
		deathAnimation.update(deltaTime, sprite, true);

		if (deathAnimation.isFinished()) {
			exploding = false;
			playDeathSoundOnce = true;
		}
	}
}

void Player::render(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Player::shoot(std::function<void(Projectile*)> push_projectile)
{
	if (canShoot() && !exploding) { // || !exploded
		shootSound.play();
		sf::Vector2f pos = sprite.getPosition() + (sprite.getGlobalBounds().getSize() / 2.f);
		pos.y -= 10.f;
		pos.x -= 1.f;

		push_projectile(ProjectileFactory::createProjectile(pos, atlas, "player_projectile", 1, -1));

		shootTimer = 0.f;  // Reset timer after shooting
	}
}

void Player::explode(bool gameOver)
{
	if (!gameOver) {
		lives--;
	}
	else {
		lives = 0;
	}
	exploding = true;
}

sf::Vector2f Player::getPosition() {
	return sprite.getPosition();
}

