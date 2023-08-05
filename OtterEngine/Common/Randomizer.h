#pragma once

#include <random>

class Randomizer {
public:
	static void Init() {
		s_random.seed(std::random_device{}());
	}

	// int [0, INT_MAX]
	static int GetInt() {
		std::uniform_int_distribution<int> intDist;
		return intDist(s_random);
	}

	// int [min, max]
	static int GetInt(int max, int min = 0) {
		std::uniform_int_distribution<int> intDist(min, max);
		return intDist(s_random);
	}

	// float [0.0, 1.0]
	static float GetFloat() {
		std::uniform_real_distribution<float> distributionFloat;
		return distributionFloat(s_random);
	}

	// float [min, max]
	static float GetFloat(float max, float min = 0.0) {
		std::uniform_real_distribution<float> distributionFloat(min, max);
		return distributionFloat(s_random);
	}

private:
	static std::mt19937 s_random;
};

std::mt19937 Randomizer::s_random;