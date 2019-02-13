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
	}
}