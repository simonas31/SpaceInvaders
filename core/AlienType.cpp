#pragma once
#include <string>

struct AlienType {
	int alienScore;
	std::string alienName;
	AlienType(const std::string& alienName, int alienScore) : alienName(alienName), alienScore(alienScore) {}
};