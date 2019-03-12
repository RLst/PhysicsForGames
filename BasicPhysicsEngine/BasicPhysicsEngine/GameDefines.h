#pragma once
#include <glm/ext.hpp>
#include <algorithm>
#include "Random.h"
//Constants
const float PI = 3.14159f;

//Usings
using glm::vec2;
using glm::vec3;
using glm::vec4;

//Typedefs
typedef glm::vec4 col;

namespace pkr
{
	class Random;

	//Utility functions
	static float min(float a, float b) { return a < b ? a : b; }
	static float max(float a, float b) { return a > b ? a : b; }
	static float clamp(float val, float minVal, float maxVal) { return max(minVal, min(maxVal, val)); }
	static float DegsToRad(float degrees) { return degrees * PI / 180.0f; }
	static float RadsToDeg(float radians) { return radians * 180.0f / PI; }

	//Zero values
	const vec2 zero2 = vec2(0, 0);
	const vec3 zero3 = vec3(0, 0, 0);
	const vec4 zero4 = vec4(0, 0, 0, 0);

	//Colour stuff
	enum eColours {
		WHITE,
		RED,
		GREEN,
		BLUE,
		CYAN,
		MAGENTA,
		YELLOW,
		ORANGE,
		LIMEGREEN,
		MINT,
		DODGERBLUE,
		INDIGO,
		PURPLE,
		FUSCHIA,
		GREY,
		COLOUR_COUNT
	};

	class colour
	{
	private:
		colour() = default;
	public:
		static col get(eColours colour)
		{
			switch (colour)
			{
			case WHITE: return col(1, 1, 1, 1); break;
			case RED: return col(1, 0, 0, 1); break;
			case GREEN: return col(0.1f, 0.9f, 0, 1); break;
			case BLUE: return col(0, 0, 1, 1); break;
			case CYAN: return col(0, 1, 1, 1); break;
			case MAGENTA: return col(1, 0, 1, 1); break;
			case YELLOW: return col(1, 0.85f, 0, 1); break;
			case ORANGE: return col(1, 0.5f, 0, 1); break;
			case LIMEGREEN: return col(0.75f, 1, 0, 1); break;
			case MINT: return col(0, 1, 0.5f, 1); break;
			case DODGERBLUE: return col(0, 0.5f, 1, 1); break;
			case INDIGO: return col(0.25f, 0, 1, 1); break;
			case PURPLE: return col(0.5f, 0, 1, 1); break;
			case FUSCHIA: return col(1, 0, 0.5f, 1); break;
			case GREY: return col(0.5f, 0.5f, 0.5f, 1); break;
			default: return col(1, 1, 1, 0.5f); break;
			}
		}

		static col random()
		{
			int rnd = pkr::Random::range(0, eColours::COLOUR_COUNT - 1);
			return get((eColours)rnd);
		}

		static col nice_random()
		{
			int rnd = pkr::Random::range(0, 6);
			switch (rnd)
			{
			case 0: return get(GREEN); break;
			case 1: return get(RED); break;
			case 2: return get(YELLOW); break;
			case 3: return get(ORANGE); break;
			case 4: return get(DODGERBLUE); break;
			case 5: return get(PURPLE); break;
			case 6: return get(FUSCHIA); break;
			default: return get(WHITE);	//default to white
			}
		}

		static col shade(float intensity)
		{
			intensity = clamp(intensity, 0, 1);
			return col(intensity, intensity, intensity, 1);
		}
	};
}