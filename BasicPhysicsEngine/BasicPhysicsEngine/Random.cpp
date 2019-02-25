#include "Random.h"

namespace pkr {
	std::random_device		Random::m_rd;
	std::mt19937			Random::m_engine(m_rd());
	std::mt19937			Random::m_det_engine;
	
	//Non deterministic
	//Integers
	int Random::range(int min, int max) {
		std::uniform_int_distribution<int> dist(min, max);
		return dist(m_engine);
	}
	//Floats
	float Random::range(float min, float max) {	
		std::uniform_real_distribution<float> dist(min, max);
		return dist(m_engine);
	}
	//glm::vec2
	glm::vec2 Random::range_v2(float min, float max) {	
		std::uniform_real_distribution<float> dist(min, max);
		return glm::vec2(dist(m_engine), dist(m_engine));
	}

	//Deterministic
	//Integers
	int Random::range_det(int min, int max) {
		std::uniform_int_distribution<int> dist(min, max);
		return dist(m_det_engine);
	}
	//Floats
	float Random::range_det(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return dist(m_det_engine);
	}
	//glm::vec2
	glm::vec2 Random::range_v2_det(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return glm::vec2(dist(m_det_engine), dist(m_det_engine));
	}
}