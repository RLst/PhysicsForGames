#pragma once
#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
protected:
	glm::vec2	m_position;
	glm::vec2	m_velocity;
	float		m_mass;
	float		m_rotation;	//2D so we only need a single float to represent our rotation

public:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass);
	virtual ~RigidBody();

	void		FixedUpdate(glm::vec2 gravity, float timeStep) override;
	void		Debug() override;

	void		ApplyForce(glm::vec2 force);
	void		ApplyForceToActor(RigidBody* actor2, glm::vec2 force);

	virtual bool CheckCollision(PhysicsObject* other) = 0;
	void		ResolveCollision(RigidBody* other);

	glm::vec2	getPosition() const { return m_position; }
	float		getRotation() const { return m_rotation; }
	glm::vec2	getVelocity() const { return m_velocity; }
	float		getMass() const { return m_mass; }

	void		reduceMass(float reduction) { m_mass -= reduction; }

	//TEMP
	void		tempSetVelocity(const glm::vec2 vel) { m_velocity = vel; }
};

