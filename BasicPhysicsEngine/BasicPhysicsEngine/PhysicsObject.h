#pragma once

#include <glm/ext.hpp>

enum ShapeType
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
	ShapeType m_shapeID;
	PhysicsObject(ShapeType shapeID) : m_shapeID(shapeID) {}
public:
	virtual ~PhysicsObject() noexcept;	//Ran into an error

	virtual void	FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void	Debug() = 0;
	virtual void	MakeGizmo() = 0;
	virtual void	ResetPosition() {};

	ShapeType		GetShapeID() { return m_shapeID; }
};

