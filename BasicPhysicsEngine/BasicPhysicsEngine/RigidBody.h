#pragma once
#include "PhysicsObject.h"

#include <algorithm>

class RigidBody : public PhysicsObject
{
protected:
	//Mass
	float		m_mass;

	//Displacement
	vec2	m_position;
	float		m_rotation;		//2D so we only need a single float to represent our rotation
	
	//Velocity
	vec2	m_velocity;
	float		m_angularVelocity;

	//Drag
	float		m_linearDrag;
	float		m_angularDrag;
	const float MIN_LINEAR_THRESHOLD;		//Why can't this be static?
	const float MIN_ANGULAR_THRESHOLD;

	//Restitution
	float		m_elasticity;

	/*//Unity inspired properties
	BodyType: ie. Dynamic, Static, Kinetic
	Material: PhysicsMaterial
	Simulated?
	Mass V
	LinearDrag V
	AngularDrag
	GravityScale > gravityMultiplier
	Collision Detection: Discrete, continuous
	Sleeping mode: Never sleep, start awake, start alseep
	Interpolation: None, Interpolate, Exterpolate
	Constraints: position x, y, rotation z
	*/

public:
	RigidBody(eShapeType shapeID,

		const vec2& position,
 const vec2& velocity,
		float rotation,
 float mass,
		float elasticity = 1.f,
 float linearDrag = 0.05f,

		float angDrag = 0.15f,
		float minLinearThreshold = 0.1f, float minAngularThreshold = 0.01f);
	virtual ~RigidBody();

	void		FixedUpdate(const vec2& gravity, float timeStep) override;
	void		Debug() override;

	void		ApplyForce(const vec2& newForce);
	void		ApplyForceToActor(RigidBody* actor2, const vec2& newForce);

	//virtual bool	CheckCollision(PhysicsObject* other) = 0;	//TODO Is this even required???
	void		ResolveCollision(RigidBody* other);

	//Properties
	float		mass() const { return m_mass; }
	void		reduceMass(const float reduction) { m_mass -= reduction; }	//Used for rocket sim; unrealistic
	vec2		position() const { return m_position; }
	float		rotation() const { return m_rotation; }
	vec2		velocity() const { return m_velocity; }
	float		angVel() const { return m_angularVelocity; }

	//Drag
	float		getLinearDrag() const { return m_linearDrag; }
	void		setLinearDrag(const float lDrag) { m_linearDrag = std::clamp(lDrag, 0.0f, 1.0f); }
	float		getAngularDrag() const { return m_angularDrag; }
	void		setAngularDrag(const float angDrag) { m_angularDrag = std::clamp(angDrag, 0.0f, 1.0f); }

	//Elasticity
	float		getElasticity() const { return m_elasticity; }
	void		setElasticity(const float elasiticity) { m_elasticity = elasiticity; }

	//TEST
	void		displace(const vec2& displacement);		//To prevent sticking etc.
};

