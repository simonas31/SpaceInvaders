#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
private:
	std::vector<sf::IntRect> frames;
	unsigned int currentFrame;
	float frameTime, elapsedTime;
	bool loop = false, finished = false;
	int loops = 3, loopCount = 0;

public:
	Animation() : currentFrame(0), frameTime(0.0f), elapsedTime(0.0f) {}
	Animation(float frameTime) : currentFrame(0), frameTime(frameTime), elapsedTime(0.0f) {}

	void addFrame(const sf::IntRect& frame);
	void update(float deltaTime, sf::Sprite& sprite, bool updateInstantly = false);
	void setLoops(int loop_iterations);
	void enableLoop();
	void disableLoop();
	bool isFinished();
	void reset();
};
