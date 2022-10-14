#pragma once
#include <random>

class RandomNumberGenerator
{
private:
	std::mt19937 m_engine;
	unsigned int m_seed;
public:
	// Default constructor, generates own seed
	RandomNumberGenerator();
	// Constructor with given seed
	RandomNumberGenerator(unsigned int seed);

	// Returns an int in specified range
	int RandomInt(int rangeLow, int rangeHigh);

	// Returns a float in a specified range
	float RandomFloat(float rangeLow, float rangeHigh);

	// Returns rngs seed
	unsigned int GetSeed() { return m_seed; }
};