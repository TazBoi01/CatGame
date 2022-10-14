#include "RandomNumberGenerator.h"
#include <random>

RandomNumberGenerator::RandomNumberGenerator()
	: m_engine()
	, m_seed()
{
	// Set random seed
	std::random_device randomDevice;
	m_seed = randomDevice();
	// Seed engine
	m_engine.seed(m_seed);
}

RandomNumberGenerator::RandomNumberGenerator(unsigned int seed)
	: m_seed(seed)
{
	// Seed engine
	m_engine.seed(m_seed);
}

int RandomNumberGenerator::RandomInt(int rangeLow, int rangeHigh)
{
	// Subtract low from high to add it back after mod
	int randomNumber = (m_engine() % ((rangeHigh + 1) - rangeLow)) + rangeLow;
	return randomNumber;
}

float RandomNumberGenerator::RandomFloat(float rangeLow, float rangeHigh)
{
	// Return float
	std::uniform_real_distribution<float> randFloat(rangeLow, rangeHigh);
	return randFloat(m_engine);
}
