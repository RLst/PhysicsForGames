#pragma once
#include <glm/ext.hpp>


////////////////
// Type defs //
//////////////
typedef glm::vec2 v2;
typedef glm::vec4 Col;


namespace pkr
{
	namespace colour
	{
		Col red = Col(1, 0, 0, 1);
		Col blue = Col(0, 1, 0, 1);
		Col green = Col(0, 0, 1, 1);
		Col yellow = Col(1, 1, 0, 1);
		Col orange = Col(1, 0.5f, 0, 1);
		Col cyan = Col(0, 1, 1, 1);
		Col violet = Col(1, 0, 1, 1);
		Col white = Col(1, 1, 1, 1);
		Col gray = Col(0.5f, 0.5f, 0.5f, 1);
	}
}