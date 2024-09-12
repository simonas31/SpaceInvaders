#include "Animation.h"

void Animation::addFrame(const sf::IntRect& frame) {
	frames.push_back(frame);
}

void Animation::update(float deltaTime, sf::Sprite& sprite, bool updateInstantly) {
	elapsedTime += deltaTime;

	if (loop) {
		// Check if it's time to switch to the next frame
		bool switchFrame = elapsedTime >= (frameTime / (frames.size() * loops));
		if (switchFrame || updateInstantly) {
			if(switchFrame)
				elapsedTime = 0.0f;

			// If we reached the last frame, reset the currentFrame and increment the loop count
			if (currentFrame >= frames.size()) {
				currentFrame = 0; // Reset frame index to loop
				loopCount++;	  // Increment loop count
				return;
			}

			sprite.setTextureRect(frames[currentFrame]);
			if (switchFrame)
				currentFrame++;
		}
	}
	else {
		// infinite looping
		if (elapsedTime >= frameTime) {
			elapsedTime = 0.0f;
			sprite.setTextureRect(frames[currentFrame]);
			currentFrame = (currentFrame + 1) % frames.size();
		}
	}
}

void Animation::setLoops(int loop_iterations)
{
	//do not let loop iterations be 0, because in update division by 0 will cause error
	if (loop_iterations == 0) {
		loops = 1;
		return;
	}
	loops = loop_iterations;
}

//enables loop for animation
void Animation::enableLoop()
{
	loop = true;
}

void Animation::disableLoop()
{
	loop = false;
}

bool Animation::isFinished() {
	bool res = loopCount >= loops;
	if (res)
		reset();
	return res;
}

void Animation::reset()
{
	loopCount = 0;
}
