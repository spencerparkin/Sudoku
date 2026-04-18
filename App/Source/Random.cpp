#include "Random.h"

Random::Random() : distribution(0.0, 1.0)
{
}

/*virtual*/ Random::~Random()
{
}

/*virtual*/ void Random::SetSeed(unsigned int seed)
{
	this->generator = std::mt19937(seed);
}

/*virtual*/ unsigned int Random::GetRandomInteger(unsigned int minInteger, unsigned int maxInteger)
{
	double alpha = this->distribution(this->generator);
	unsigned int i = (unsigned int)(double(minInteger) + alpha * double(maxInteger - minInteger));
	if (i < minInteger)
		i = minInteger;
	if (i > maxInteger)
		i = maxInteger;
	return i;
}

/*virtual*/ bool Random::CoinToss()
{
	double alpha = this->distribution(this->generator);
	return alpha > 0.5;
}