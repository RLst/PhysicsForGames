#pragma once
#include <glm/ext.hpp>

////////////////
// Type defs //
//////////////

using glm::vec2;
using glm::vec3;
using glm::vec4;

typedef glm::vec4 col;

const float PI = 3.14159f;

namespace pkr
{
	static int random(int min, int max)
	{
		return rand() % (max - min + 1) + min;
	}

	namespace colour
	{
		const col red = col(1, 0, 0, 1);
		const col blue = col(0, 1, 0, 1);
		const col green = col(0, 0, 1, 1);
		const col yellow = col(1, 1, 0, 1);
		const col orange = col(1, 0.5f, 0, 1);
		const col cyan = col(0, 1, 1, 1);
		const col violet = col(1, 0, 1, 1);
		const col white = col(1, 1, 1, 1);
		const col gray = col(0.5f, 0.5f, 0.5f, 1);

		static col random()
		{
			int rnd = pkr::random(0, 8);
			switch (rnd)
			{
			case 0: return red; break;
			case 1: return blue; break;
			case 2: return green; break;
			case 3: return yellow; break;
			case 4: return orange; break;
			case 5: return cyan; break;
			case 6: return violet; break;
			case 7: return white; break;
			case 8: return gray; break;
			default: return col(1, 1, 1, 1);
			}
		}
	}

	static float DegsToRad(float degrees) { return degrees * PI / 180.0f; }
	static float RadsToDeg(float radians) { return radians * 180.0f / PI; }

	const vec2 zero2 = vec2(0, 0);
	const vec3 zero3 = vec3(0, 0, 0);
	const vec4 zero4 = vec4(0, 0, 0, 0);
}
