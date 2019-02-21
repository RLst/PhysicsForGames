#pragma once
#include "PhysicsObject.h"
#include <algorithm>

class RigidBody : public PhysicsObject
{
protected:
	//Mass
	float		m_mass;

	//Displacement
	glm::vec2	m_position;
	float		m_rotation;		//2D so we only need a single float to represent our rotation
	
	//Velocity
	glm::vec2	m_velocity;
	float		m_angularVelocity;

	//Drag
	float		m_linearDrag;
	float		m_angularDrag;
	const float MIN_LINEAR_THRESHOLD;		//Why can't this be static?
	const float MIN_ANGULAR_THRESHOLD;

	//Restitution
	float		m_elasticity;

public:
	RigidBody(ShapeType shapeID,
		glm::vec2 position, glm::vec2 velocity,
		float rotation, float mass,
		float linearDrag = 0.15f, float angDrag = 0.15f,	//Default drags
		float elasticity = 1.0f,
		float minLinearThreshold = 0.1f, float minAngularThreshold = 0.01f);
	virtual ~RigidBody();

	void		FixedUpdate(glm::vec2 gravity, float timeStep) override;
	void		Debug() override;

	void		ApplyForce(glm::vec2 force);
	void		ApplyForceToActor(RigidBody* actor2, glm::vec2 force);

	//virtual bool	CheckCollision(PhysicsObject* other) = 0;	//TODO Is this even required???
	void			ResolveCollision(RigidBody* other);

	//Internal use
	void		displace(const glm::vec2 displacement);	//To prevent sticking etc

	//Other / Tests
	void		setVelocity(const glm::vec2 vel) { m_velocity = vel; }

	//Properties
	float		getMass() const { return m_mass; }
	void		reduceMass(const float reduction) { m_mass -= reduction; }	//Used for rocket sim; unrealistic
	glm::vec2	getPosition() const { return m_position; }
	float		getRotation() const { return m_rotation; }
	glm::vec2	getVelocity() const { return m_velocity; }
	float		getAngVelocity() const { return m_angularVelocity; }

	float		getLinearDrag() const { return m_linearDrag; }
	void		setLinearDrag(const float lDrag) { m_linearDrag = std::clamp(lDrag, 0.0f, 1.0f); }
	float		getAngularDrag() const { return m_angularDrag; }
	void		setAngularDrag(const float angDrag) { m_angularDrag = std::clamp(angDrag, 0.0f, 1.0f); }

	float		getElasticity() const { return m_elasticity; }
	void		setElasticity(const float elasiticity) { m_elasticity = elasiticity; }
};

