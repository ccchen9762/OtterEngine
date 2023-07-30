#pragma once

#include <random>

class Randomizer {
public:
	static void Init() { 
		s_random.seed(std::random_device()()); 
	}
	
	static uint32_t GetInt() { 
		return s_distributionInt(s_random);
	}
	
	static uint32_t GetInt(uint32_t max, uint32_t min = 0) {
		return min + (s_distributionInt(s_random) % (max - min + 1));
	}

	static double GetFloat() {
		return s_distributionDouble(s_random);
	}

	static double GetFloat(uint32_t max, uint32_t min = 0) {
		return min + (s_distributionDouble(s_random) * (max - min));
	}

private:
	static std::mt19937 s_random;
	static std::uniform_int_distribution<uint32_t> s_distributionInt;
	static std::uniform_real_distribution<double> s_distributionDouble;
};