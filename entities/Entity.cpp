#pragma once
#include "../core/SpriteAtlas.h"
#include "../projectiles/Projectile.h"
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Entity
{
protected:
	Animation animation;
	Animation deathAnimation;
	sf::Sprite sprite;

	float speed;
	int lives;

	float shootCooldown = 1.5f;  // Time between each shot
	float shootTimer = .0f;     // Timer for the cooldown

	bool exploded = false, exploding = false;  // New flag to track if entity exploded
	bool playDeathSoundOnce = true;

	SpriteAtlas* atlas;
	sf::SoundBuffer movementBuffer;
	sf::SoundBuffer shootBuffer;
	sf::SoundBuffer deathBuffer;
	sf::Sound movementSound;
	sf::Sound shootSound;
	sf::Sound deathSound;

	void setSprite(const std::string& name) {
		sprite = atlas->createSprite(name);
		sprite.setScale(sf::Vector2f(2.f, 2.f));
	}
public:
	virtual ~Entity() = default;

	// Update entity movement or logic
	virtual void update(float deltaTime) = 0;

	// Draw the entity on the window
	virtual void render(sf::RenderWindow& window) {
		window.draw(sprite);
	}

	virtual bool canShoot() { return shootTimer >= shootCooldown; }
	// shoot function
	virtual void shoot(std::function<void(Projectile*)> push_projectile) = 0;

	sf::FloatRect getBounds() const {
		return sprite.getGlobalBounds();
	};

	bool isAlive() const {
		return lives > 0 || exploding;
	}

	bool isExploding() const {
		return exploding;
	}

	// Get position
	sf::Vector2f getPosition() const {
		return sprite.getPosition();
	}

	int getLives() const { return lives; }
	sf::Sprite getSprite() const { return sprite; }
};

