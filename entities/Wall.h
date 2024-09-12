#pragma once
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "../core/SpriteAtlas.h"
#include "../projectiles/Projectile.h"

class Wall
{
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    SpriteAtlas* atlas;
public:
	Wall(sf::Vector2f position, SpriteAtlas* atlas);
    // Update game object's logic
    void update(float deltaTime);

    // Draw the object on the window
    void render(sf::RenderWindow& window);

    void destroy(sf::FloatRect bounds);

    bool intersects(sf::FloatRect bounds);

    sf::IntRect getSpriteCollision(sf::FloatRect bounds);

    // Get the object's global bounding box (used for collision)
    sf::FloatRect getBounds() const;
};

