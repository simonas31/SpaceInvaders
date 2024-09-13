#include "../core/Game.h"
#include "../entities/Player.h"
#include "../entities/Alien.h"
#include "../entities/Entity.cpp"
#include "../entities/Wall.h"

void Game::setupSpriteAtlas()
{
	spriteSheet.loadFromFile("assets/sprites/space_invaders_spritesheet.png");
	atlas = SpriteAtlas(&spriteSheet);

	//player
	atlas.addSpriteLocation("player", 0, 36, 13, 8);
	atlas.addSpriteLocation("player_projectile_1", 15, 36, 1, 8);

	//android alien
	atlas.addSpriteLocation("android_alien_sprite_1", 0, 9, 11, 8);
	atlas.addSpriteLocation("android_alien_sprite_2", 12, 9, 11, 8);
	atlas.addSpriteLocation("android_alien_projectile_1", 24, 9, 3, 8);
	atlas.addSpriteLocation("android_alien_projectile_2", 28, 9, 3, 8);
	atlas.addSpriteLocation("android_alien_projectile_3", 32, 9, 3, 8);
	atlas.addSpriteLocation("android_alien_projectile_4", 36, 9, 3, 8);

	//skull alien
	atlas.addSpriteLocation("skull_alien_sprite_1", 0, 18, 12, 8);
	atlas.addSpriteLocation("skull_alien_sprite_2", 13, 18, 12, 8);
	atlas.addSpriteLocation("skull_alien_projectile_1", 26, 18, 3, 8);
	atlas.addSpriteLocation("skull_alien_projectile_2", 30, 18, 3, 8);
	atlas.addSpriteLocation("skull_alien_projectile_3", 34, 18, 3, 8);
	atlas.addSpriteLocation("skull_alien_projectile_4", 38, 18, 3, 8);

	//squid alien
	atlas.addSpriteLocation("squid_alien_sprite_1", 0, 0, 8, 8);
	atlas.addSpriteLocation("squid_alien_sprite_2", 9, 0, 8, 8);
	atlas.addSpriteLocation("squid_alien_projectile_1", 18, 0, 3, 8);
	atlas.addSpriteLocation("squid_alien_projectile_2", 22, 0, 3, 8);
	atlas.addSpriteLocation("squid_alien_projectile_3", 26, 0, 3, 8);
	atlas.addSpriteLocation("squid_alien_projectile_4", 30, 0, 3, 8);

	//death
	atlas.addSpriteLocation("entity_death_1", 0, 27, 13, 8);
	atlas.addSpriteLocation("entity_death_2", 13, 27, 13, 8);

	//projectile explosion
	atlas.addSpriteLocation("projectile_explosion_1", 18, 36, 6, 8);
	atlas.addSpriteLocation("projectile_explosion_2", 24, 36, 6, 8);

	//wall
	atlas.addSpriteLocation("wall", 0, 45, 22, 16);

	//mystery ship
	atlas.addSpriteLocation("mystery_ship", 0, 62, 16, 8);
}

Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(window_W, window_H), "Space Invaders");
	setupSpriteAtlas();
	initializeText();
	setupLevel();
}

Game::Game(int width, int height, const char* title)
{
	window_W = width;
	window_H = height;
	window = new sf::RenderWindow(sf::VideoMode(window_W, window_H), title);
	setupSpriteAtlas();
	initializeText();
	setupLevel();
}

Game::~Game() {}

void Game::run()
{
	while (window->isOpen()) {
		float deltaTime = clock.restart().asSeconds();
		if (initializingLevel) {
			initializeLevel(deltaTime);
		}
		else {
			processEvents();
			update(deltaTime);
			render();
		}

		if (aliens.empty()) {
			setupLevel();
			reset();
		}
	}
}

void Game::update(float deltaTime)
{
	// IF GAME OVER THEN DISPLAY GAME OVER TEXT, SAVE SCORE AND GO TO DASHBOARD
	if (player->getLives() == 0) {
		gameOver = true;
		if (mysteryShip)
			mysteryShip->stopSounds();
	}

	if (gameOver && clickedRestart()) {
		score = 0;
		updateScore(score);
		setupLevel();
		gameOver = false;
		reset();
	}

	if (gameOver || player->isExploding()) {
		player->update(deltaTime);
		return;
	}

	if (paused)
		return;

	updateProjectiles(deltaTime);

	player->update(deltaTime);

	spawnMysteryShip(deltaTime);
	updateMysteryShip(deltaTime);

	updateAliens(deltaTime);

	updateWalls(deltaTime);

	//check for collision
	checkForCollisions();
}

void Game::render()
{
	window->clear();

	//draw rect when user hovers text "RESTART"
	if (gameOver) {
		sf::FloatRect textBounds = texts["restart_text"]->getGlobalBounds();
		if (textBounds.contains((sf::Vector2f)sf::Mouse::getPosition(*window))) {
			sf::RectangleShape rect = createTextBorder(textBounds, sf::Color::White, 3.f);
			window->draw(rect);
		}
	}

	// render walls
	for (Wall* wall : walls) {
		wall->render(*window);
	}

	// render aliens
	for (Alien* alien : aliens) {
		alien->render(*window);
	}

	renderProjectiles();

	// render player
	player->render(*window);

	if (mysteryShip != nullptr) {
		mysteryShip->render(*window);
	}

	// render text;
	for (auto itr = texts.begin(); itr != texts.end(); ++itr) {
		const std::string& key = itr->first;
		sf::Text* text = itr->second;

		if (key == "paused") {
			text->setFillColor(paused ? sf::Color::Green : sf::Color::Transparent);
		}
		else if (key == "game_over") {
			text->setFillColor(gameOver ? sf::Color::Red : sf::Color::Transparent);
		}
		else if (key == "restart_text") {
			text->setFillColor(gameOver ? sf::Color::White : sf::Color::Transparent);
		}

		// Draw the text if its not transparent
		if (text->getFillColor() != sf::Color::Transparent) {
			window->draw(*text);
		}
	}

	// render lives
	sf::Sprite sp = player->getSprite();
	for (int i = 0; i < player->getLives(); i++) {
		sp.setPosition(sf::Vector2f((float)window_W - 130.f + i * 35.f, 10.f));
		window->draw(sp);
	}

	window->display();
}

void Game::processEvents()
{
	sf::Event e;
	while (window->pollEvent(e)) {
		if (gameOver)
			continue;

		if (e.type == sf::Event::Closed)
			window->close();
		if ((e.type == sf::Event::KeyReleased) && (e.key.code == sf::Keyboard::Escape))
			paused = !paused;

		if (paused)
			continue;

		if (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::Space) {
			player->shoot([this](Projectile* p) {
				playerProjectiles.push_back(p);
				});  // Add player's bullet to the list
		}
	}
}

void Game::initializeText()
{
	font.loadFromFile("assets/fonts/space_invaders.ttf");
	sf::Text* pausedText = new sf::Text("PAUSED", font, 24);
	pausedText->setPosition(sf::Vector2f((float)window_W / 2.f - pausedText->getGlobalBounds().width / 2.f + .5f, (float)window_H / 2.f - pausedText->getGlobalBounds().height));
	pausedText->setFillColor(sf::Color::Transparent);
	texts["paused"] = pausedText;

	sf::Text* scoreText = new sf::Text("SCORE", font, 16);
	scoreText->setPosition(sf::Vector2f(30.f, 10.f));
	texts["score_text"] = scoreText;

	std::string scoreString;
	scoreString = std::to_string(score);
	sf::Text* score = new sf::Text(scoreString, font, 16);
	score->setPosition(sf::Vector2f(100.f, 10.f));
	texts["score"] = score;

	sf::Text* livesText = new sf::Text("LIVES", font, 16);
	livesText->setPosition(sf::Vector2f(window_W - 200.f, 10.f));
	texts["lives_text"] = livesText;

	sf::Text* gameOverText = new sf::Text("GAME OVER", font, 24);
	gameOverText->setPosition(sf::Vector2f((float)window_W / 2.f - gameOverText->getGlobalBounds().width / 2.f + .5f, (float)window_H / 2.f - gameOverText->getGlobalBounds().height));
	gameOverText->setFillColor(sf::Color::Transparent);
	texts["game_over"] = gameOverText;

	sf::Text* restartText = new sf::Text("RESTART", font, 16);
	restartText->setPosition(sf::Vector2f((float)window_W / 2.f - restartText->getGlobalBounds().width / 2.f, (float)window_H / 2.f - restartText->getGlobalBounds().height + 40.f));
	restartText->setFillColor(sf::Color::Transparent);
	texts["restart_text"] = restartText;
}

void Game::setupLevel()
{
	playerProjectiles = std::vector<Projectile*>();
	alienProjectiles = std::vector<Projectile*>();
	aliens = std::vector<Alien*>();
	walls = std::vector<Wall*>();

	// Add Player
	player = new Player(sf::Vector2f((float)window_W / 2, (float)window_H - 50), &atlas);

	int i = 0;
	for (i = 0; i < 4; i++) {
		walls.push_back(new Wall(sf::Vector2f(7.f + (float)i * (float)window_W / 4.f, (float)window_H - 100.f), &atlas));
	}

	// Define aliens
	AlienType squidAlien("squid_alien", ScoreList::SquidAlienScore);
	AlienType androidAlien("android_alien", ScoreList::AndroidAlienScore);
	AlienType skullAlien("skull_alien", ScoreList::SkullAlienScore);

	// Add some Enemies
	for (int row = 0; row < 5; row++)
	{
		for (int col = 0; col < 11; col++)
		{
			float x = 30.f + (float)col * 24.f;
			float y = 110.f + (float)row * 24.f;

			if (row == 0) {
				aliens.push_back(new Alien(squidAlien, sf::Vector2f(x + (float)col * 5 + 4.f, y), &atlas, col));
			}
			else if (row <= 2) {
				aliens.push_back(new Alien(androidAlien, sf::Vector2f(x + (float)col * 5 + 1.f, y), &atlas, col));
			}
			else {
				aliens.push_back(new Alien(skullAlien, sf::Vector2f(x + (float)col * 5, y), &atlas, col));
			}
		}
	}

	if (mysteryShip) {
		delete mysteryShip;
		mysteryShip = nullptr;
	}
	else {
		mysteryShip = nullptr;
	}
}

void Game::updateWalls(float deltaTime)
{
	for (Wall* wall : walls) {
		wall->update(deltaTime);
	}
}

// update aliens
void Game::updateAliens(float deltaTime)
{
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	std::unordered_map<int, Alien*> lowestAlienInColumn;
	//check if we need to move down
	//also find lowest alien position in column that would shoot projectile
	for (Alien* alien : aliens) {
		int col = alien->getColumn();

		if (lowestAlienInColumn.find(col) == lowestAlienInColumn.end() || alien->getPosition().y > lowestAlienInColumn[col]->getPosition().y) {
			lowestAlienInColumn[col] = alien;
		}

		if (alien->isOffBoundaries((float)window_W)) {
			changeAliensDirection = true;
		}
	}

	// Update all entities
	for (auto a = aliens.begin(); a != aliens.end();) {
		if (changeAliensDirection) {
			(*a)->reverseDirectrion();
		}
		(*a)->update(deltaTime);

		// shoot only when in first row or below row column cells are empty
		// shoot randomly
		if (lowestAlienInColumn[(*a)->getColumn()] != nullptr && lowestAlienInColumn[(*a)->getColumn()] == (*a) && (std::rand() % 5) == 4) {
			(*a)->shoot([this](Projectile* p) {
				p->setSpeed(100.f);
				alienProjectiles.push_back(p);
				});
		}

		if (!(*a)->isAlive()) {
			delete* a;
			a = aliens.erase(a);
			destroyedAliensCount++;
		}
		else {
			if (!changeAliensDirection)
				(*a)->setSpeedFactor(calculateAliensSpeed((int)aliens.size()));
			++a;
		}
	}

	if (changeAliensDirection)
		changeAliensDirection = false;
}

void Game::updateProjectiles(float deltaTime)
{
	const float window_H = static_cast<float>(window->getSize().y);

	for (auto p = playerProjectiles.begin(); p != playerProjectiles.end();) {
		// If the projectile is off the boundaries, trigger the explosion
		if ((*p)->isOffBoundaries(window_H) && !(*p)->isExploding()) {
			(*p)->explode();
		}

		(*p)->update(deltaTime);

		// If explosion is finished, delete the projectile
		if ((*p)->isExploded()) {
			delete* p;
			p = playerProjectiles.erase(p);
		}
		else {
			++p;
		}
	}

	for (auto a = alienProjectiles.begin(); a != alienProjectiles.end();) {
		// If the projectile is off the boundaries, trigger the explosion
		if ((*a)->isOffBoundaries(window_H) && !(*a)->isExploding()) {
			(*a)->explode();
		}

		(*a)->update(deltaTime);

		// If explosion is finished, delete the projectile
		if ((*a)->isExploded()) {
			delete* a;
			a = alienProjectiles.erase(a);
		}
		else {
			++a;
		}
	}
}

void Game::updateMysteryShip(float deltaTime)
{
	if (mysteryShip) {  // Check if mysteryShip is not nullptr
		if (mysteryShip->getPosition().x >= -50.f && mysteryShip->isAlive()) {
			mysteryShip->update(deltaTime);
		}
		else {
			delete mysteryShip;
			mysteryShip = nullptr;
		}
	}
}

void Game::renderProjectiles()
{
	for (Projectile* pp : playerProjectiles) {
		pp->render(*window);
	}

	for (Projectile* ap : alienProjectiles) {
		ap->render(*window);
	}
}

void Game::checkForCollisions()
{
	// player projectile - aliens, walls. collision
	for (auto& pp : playerProjectiles) {
		// if projectile is exploding then dont use it. it will be removed
		if (pp->isExploding())
			continue;
		for (auto& alien : aliens) {
			if (!alien->isExploding() && alien->getBounds().intersects(pp->getBounds())) {
				updateScore(alien->getScore());
				alien->explode();
				pp->explode();
			}
		}

		for (auto& wall : walls) {
			if (wall->intersects(pp->getBounds())) {
				pp->explode(true);
				wall->destroy(pp->getBounds());
			}
		}

		if (mysteryShip && !mysteryShip->isExploding() && pp->getBounds().intersects(mysteryShip->getBounds())) {
			updateScore(ScoreList::MysteryShipScore);
			mysteryShip->explode();
			pp->explode();
		}
	}

	// alien projectile - player, walls. collision
	for (auto& ap : alienProjectiles) {
		// if projectile is exploding then dont use it. it will be removed
		if (ap->isExploding())
			continue;
		if (!ap->isExploding() && !player->isExploding() && player->getBounds().intersects(ap->getBounds())) {
			player->explode();
			ap->explode();
		}

		for (auto& wall : walls) {
			if (wall->intersects(ap->getBounds())) {
				ap->explode(true);
				wall->destroy(ap->getBounds());
			}
		}
	}

	// player projectile - alien projectile. collision
	for (auto& pp : playerProjectiles) {
		if (pp->isExploding())
			continue;
		for (auto& ap : alienProjectiles) {
			if (!ap->isExploding() && pp->getBounds().intersects(ap->getBounds())) {
				updateScore(ScoreList::ProjectileScore);
				ap->explode();
				pp->explode();
			}
		}
	}

	for (auto& alien : aliens) {
		if (alien->isExploding())
			continue;

		for (auto& wall : walls) {
			if (wall->intersects(alien->getBounds())) {
				wall->destroy(alien->getBounds());
			}
		}

		if (alien->getBounds().intersects(player->getBounds())) {
			player->explode(true);
			gameOver = true;
		}
	}
}

void Game::updateScore(int entityScore)
{
	score += entityScore;
	delete texts["score"];
	std::string scoreString;
	scoreString = std::to_string(score);
	sf::Text* score = new sf::Text(scoreString, font, 16);
	score->setPosition(sf::Vector2f(100.f, 10.f));
	texts["score"] = score;
}

float Game::calculateAliensSpeed(int currentAliensCount)
{
	return MIN_ALIENS_SPEED + (MAX_ALIENS_COUNT - currentAliensCount) * 0.1f;
}

void Game::spawnMysteryShip(float deltaTime)
{
	mysteryShipSpawnerTimer += deltaTime;
	if (mysteryShip == nullptr) {
		if (mysteryShipSpawnerTimer >= 15.f) {
			mysteryShip = new MysteryShip(sf::Vector2f(-40.f, 90.f), &atlas);
			mysteryShipSpawnerTimer = .0f;
		}
	}
	else {
		mysteryShipSpawnerTimer = .0f;
	}
}

void Game::reset()
{
	elapsedTime = 0.f;
	shouldRender = true;
	renderCycles = 0;
	initializingLevel = true;
}

void Game::initializeLevel(float deltaTime)
{
	elapsedTime += deltaTime;

	if (initializingLevel) {
		if (elapsedTime >= 0.5f) {
			elapsedTime = 0.f;

			if (shouldRender) {
				render();
			}
			else {
				window->clear();
				window->display();
			}

			shouldRender = !shouldRender;
			if (!shouldRender) {
				renderCycles++;
			}

			if (renderCycles >= 3) {
				initializingLevel = false;
			}
		}
	}
}

bool Game::clickedRestart()
{
	return texts["restart_text"]->getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*window)) && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

sf::RectangleShape Game::createTextBorder(sf::FloatRect bounds, sf::Color borderColor, float borderWidth)
{
	sf::RectangleShape rect;
	rect.setPosition(sf::Vector2f(bounds.left - 5.f, bounds.top - 5.f));
	rect.setSize(sf::Vector2f(bounds.width + 10.f, bounds.height + 10.f));
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(borderColor);
	rect.setOutlineThickness(borderWidth);

	return rect;
}
