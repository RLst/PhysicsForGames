#pragma once
#include "RigidBody.h"

#include <glm/ext.hpp>
#include "GameDefines.h"
#include <vector>

class vector;
using glm::vec2;
using glm::vec4;
typedef std::vector<glm::vec2> listvec2;

class AABB : public RigidBody
{
protected:
	vec2		m_extents;	//Top right half extents
	vec4		m_colour;
public:
	AABB(const vec2& position, const vec2& velocity, float mass, float width, float height, const vec4& colour);
	~AABB();

	void		DrawGizmo() override;

	vec2		min() const { return m_position - m_extents; }	//Returns the min extent WORLD position
	vec2		max() const { return m_position + m_extents; }	//Returns the max extent WORLD positoin
	
	//Returns an array of all vertices (RELATIVE TO THE CENTER POSITION) starting from min
	listvec2	vertices() const;
	listvec2	vextents() const;

	vec2		projection(const vec2 &axis) const;			//Returns projection (x.min, x.max) in WORLD coordinates
};

