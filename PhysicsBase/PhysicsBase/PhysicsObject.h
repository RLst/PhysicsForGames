#pragma once

#include <glm/ext.hpp>

enum ShapeType
{
	PLANE = 0,
	CIRCLE,
	BOX,
};

class PhysicsObject
{
protected:
	ShapeType m_shapeID;

	PhysicsObject() {};
	PhysicsObject(ShapeType shapeID) : m_shapeID(shapeID) {}

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;
	virtual void ResetPosition() {};
};

