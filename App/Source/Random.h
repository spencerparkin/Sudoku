#pragma once

#include "UltraUtilities/Random.h"
#include <random>

/**
 * 
 */
class Random : public UU::Random
{
public:
	Random();
	virtual ~Random();

	virtual void SetSeed(unsigned int seed) override;
	virtual unsigned int GetRandomInteger(unsigned int minInteger, unsigned int maxInteger) override;
	virtual bool CoinToss() override;

private:
	std::mt19937 generator;
	std::uniform_real_distribution<double> distribution;
};