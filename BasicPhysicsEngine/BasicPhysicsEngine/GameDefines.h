#pragma once
#include <glm/ext.hpp>
#include <algorithm>

//Constants
const float PI = 3.14159f;

//Usings
using glm::vec2;
using glm::vec3;
using glm::vec4;

//Typedefs
typedef glm::vec4 col;
//typedef glm::vec2 axis2;		//???
//typedef glm::vec2 projection2;	//???
//typedef glm::vec2 point2;

namespace pkr
{
	int random(int min, int max);

	//Colour stuff
	enum Colours {
		COLOUR_RED = 0,
		COLOUR_GREEN,
		COLOUR_BLUE,
		COLOUR_CYAN,
		COLOUR_MAGENTA,
		COLOUR_YELLOW,
		COLOUR_ORANGE,
		COLOUR_LIMEGREEN,
		COLOUR_MINT,
		COLOUR_DODGERBLUE,
		COLOUR_INDIGO,
		COLOUR_PURPLE,
		COLOUR_FUSCHIA,
		COLOUR_COUNT
	};

	class colour
	{
	private:
		colour() = default;
	public:
		static col get(Colours colour)
		{
			switch (colour)
			{
			case COLOUR_RED: return col(1, 0, 0, 1); break;
			case COLOUR_GREEN: return col(0, 1, 0, 1); break;
			case COLOUR_BLUE: return col(0, 0, 1, 1); break;
			case COLOUR_CYAN: return col(0, 1, 1, 1); break;
			case COLOUR_MAGENTA: return col(1, 0, 1, 1); break;
			case COLOUR_YELLOW: return col(1, 0.75f, 0, 1); break;
			case COLOUR_ORANGE: return col(1, 0.4f, 0, 1); break;
			case COLOUR_LIMEGREEN: return col(0.75f, 1, 0, 1); break;
			case COLOUR_MINT: return col(0, 1, 0.5f, 1); break;
			case COLOUR_DODGERBLUE: return col(0, 0.5f, 1, 1); break;
			case COLOUR_INDIGO: return col(0.25f, 0, 1, 1); break;
			case COLOUR_PURPLE: return col(0.5f, 0, 1, 1); break;
			case COLOUR_FUSCHIA: return col(1, 0, 0.5f, 1); break;
			default: return col(1, 1, 1, 0.5f); break;
			}
		}

		static col random()
		{
			int rnd = pkr::random(0, Colours::COLOUR_COUNT - 1);
			return get((Colours)rnd);
		}

		static col nice_random()
		{
			int rnd = pkr::random(0, 8);
			switch (rnd)
			{
			case 0: return get(COLOUR_GREEN); break;
			case 1: return get(COLOUR_RED); break;
			case 2: return get(COLOUR_YELLOW); break;
			case 3: return get(COLOUR_ORANGE); break;
			case 4: return get(COLOUR_LIMEGREEN); break;
			case 5: return get(COLOUR_DODGERBLUE); break;
			case 6: return get(COLOUR_PURPLE); break;
			case 7: return get(COLOUR_FUSCHIA); break;
			case 8: return get(COLOUR_MINT); break;
			default: return shade(1);	//default to white
			}
		}

		static col shade(float intensity)
		{
			intensity = std::clamp(intensity, 0.0f, 1.0f);
			return col(intensity, intensity, intensity, 1);
		}
	};

	//Utilities
	static int random(int min, int max)
	{
		return rand() % (max - min + 1) + min;
	}

	static float min(float a, float b) { return a < b ? a : b; }
	static float max(float a, float b) { return a > b ? a : b; }
	static float clamp(float val, float minVal, float maxVal) { return max(minVal, min(maxVal, val)); }
	static float overlap(glm::vec2 projection1, glm::vec2 projection2) { return min(projection1.y - projection2.x, projection2.y - projection1.x); }

	static float DegsToRad(float degrees) { return degrees * PI / 180.0f; }
	static float RadsToDeg(float radians) { return radians * 180.0f / PI; }

	//Zero values
	const vec2 zero2 = vec2(0, 0);
	const vec3 zero3 = vec3(0, 0, 0);
	const vec4 zero4 = vec4(0, 0, 0, 0);
}
