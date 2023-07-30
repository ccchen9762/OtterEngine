#include "Randomizer.h"

std::mt19937 Randomizer::s_random;
std::uniform_int_distribution<uint32_t> Randomizer::s_distributionInt;
std::uniform_real_distribution<double> Randomizer::s_distributionDouble;