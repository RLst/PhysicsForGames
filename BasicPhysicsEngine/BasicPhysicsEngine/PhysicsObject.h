#pragma once

#include <glm/ext.hpp>

enum class eShapeType
{
	PLANE = 0,
	CIRCLE,
	BOX,
	SAT,
	SHAPE_COUNT
};

class PhysicsObject
{
protected:
	eShapeType m_shapeID;
	PhysicsObject(eShapeType shapeID) : m_shapeID(shapeID) {}
public:
	virtual ~PhysicsObject() noexcept;	//Ran into an error

	virtual void	FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void	Debug() = 0;
	virtual void	DrawGizmo() = 0;
	virtual void	ResetPosition() {};

	eShapeType		GetShapeID() { return m_shapeID; }
};

