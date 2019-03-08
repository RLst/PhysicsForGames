#pragma once
#include "RigidBody.h"

class PhysicsMaterial;

class AABB : public RigidBody
{
protected:
	vec2		m_extents;	//Top right half extents
	vec4		m_colour;
public:
	AABB(const vec2& position, const vec2& velocity, float mass, float width, float height, const vec4& colour);

	AABB(const vec2& pos, const vec2& vel, float width, float height, vec4 colour, PhysicsMaterial* material, bool isKinematic = false);
	~AABB();

	void		DrawGizmo() override;

	vec2		min() const { return m_position - m_extents; }	//Returns the min extent WORLD position
	vec2		max() const { return m_position + m_extents; }	//Returns the max extent WORLD positoin
	
	//Returns an array of all vertices (RELATIVE TO THE CENTER POSITION) starting from min
	vec2array	vertices() const;
	vec2array	vextents() const;

	vec2		projection(const vec2 &axis) const;			//Returns projection (x.min, x.max) in WORLD coordinates
};

