#pragma once

#include <glm/ext.hpp>
#include <random>

namespace pkr
{
	class Random
	{	//A proper random number generator
	
	//23 Feb 2019 - This is written as a static class. 
	//Maybe make it an instance class too later on
	private:
		static std::random_device	m_rd;
		static std::mt19937			m_engine;		//Non-deterministic
		static std::mt19937			m_det_engine;	//Deterministic 

	public:	//Static class
		Random() {}
		~Random() = default;

		//Range parameters are inclusive
		//Integers
		static int range(const int min, const int max);	
		static int range_det(const int min, const int max);

		//Floats
		static float range(const float min, const float max);
		static float range_det(const float min, const float max);

		//glm::vec2
		static glm::vec2 range_v2(const float min, const float max);
		static glm::vec2 range_v2_det(const float min, const float max);
	};
}


//private:
//	//std::default_random_engine m_engine;
//	std::mt19937 m_mt19937;
//	std::uniform_int_distribution<int> m_int_distrib;
//	std::uniform_int_distribution<int>::param_type m_int_params;
//	std::uniform_real_distribution<float> m_float_distrib;
//	std::uniform_real_distribution<float>::param_type m_float_params;
//public:
//	random() : m_int_params(0, INT_MAX), m_float_params(0.0f, 1.0f) {}
//	void seed(unsigned int value) { m_mt19937.seed(value); }
//	int next() {
//		//Reset parameters
//		m_int_params._Min = 0;
//		m_int_params._Max = INT_MAX;
//		m_int_distrib.param(m_int_params);
//		return m_int_distrib(m_mt19937);
//	}
//	int range(int min, int max) {
//		//set distribution range
//		m_int_params._Min = min;
//		m_int_params._Max = max;
//		m_int_distrib.param(m_int_params);
//		//Generate value
//		return m_int_distrib(m_mt19937);
//	}