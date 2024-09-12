#pragma once
#include "../core/SpriteAtlas.h"
#include "../entities/Alien.h"
#include "../entities/Player.h"
#include "../entities/Wall.h"
#include "../projectiles/Projectile.h"
#include "../entities/MysteryShip.h"
#include "../core/AlienType.cpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cstdlib>
#include <ctime>

#define MAX_ALIENS_SPEED 6.f
#define MIN_ALIENS_SPEED 0.5f
#define MAX_ALIENS_COUNT 55

enum ScoreList {
	MysteryShipScore = 100,
	SquidAlienScore = 30,
	AndroidAlienScore = 20,
	SkullAlienScore = 10,
	ProjectileScore = 5
};

class Game
{
private:
	int window_W = 640, window_H = 480;
	bool paused = false, gameOver = false;
	sf::RenderWindow* window;
	sf::Clock clock;

	sf::Font font;
	std::map<std::string, sf::Text*> texts;
	sf::Texture spriteSheet;
	SpriteAtlas atlas;

	Player* player;
	MysteryShip* mysteryShip;
	std::vector<Wall*> walls;
	std::vector<Alien*> aliens;
	std::vector<Projectile*> playerProjectiles;
	std::vector<Projectile*> alienProjectiles;

	unsigned int score = 0;

	bool changeAliensDirection = false;
	bool initializingLevel = true, shouldRender = true;
	int destroyedAliensCount = 0, renderCycles = 0;

	float mysteryShipSpawnerTimer = 0.f;

	float elapsedTime = .0f;

	void setupSpriteAtlas();
public:
	Game();
	Game(int width, int height, const char* title);
	~Game();
	void run();
	void update(float deltaTime);
	void render();
	void processEvents();
	void initializeText();
	void setupLevel();
	void updateWalls(float deltaTime);
	void updateAliens(float deltaTime);
	void updateProjectiles(float deltaTime);
	void updateMysteryShip(float deltaTime);
	void renderProjectiles();
	void checkForCollisions();
	void updateScore(int entityScore);
	float calculateAliensSpeed(int currentAliensCount);
	void spawnMysteryShip(float deltaTime);
	void reset();
	void initializeLevel(float deltaTime);
};