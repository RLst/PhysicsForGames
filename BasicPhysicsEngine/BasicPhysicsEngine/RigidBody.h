#pragma once
#include "PhysicsObject.h"
#include <algorithm>
#include "PhysicsMaterial.h"

class RigidBody : public PhysicsObject
{
protected:
	bool		m_isKinematic;

	//Linear
	float		m_mass;
	vec2		m_position;
	vec2		m_velocity;
	float		m_linearDrag;
	const float MIN_LINEAR_THRESHOLD;		//Why can't this be static?

	//Angular
	float		m_moment;
	float		m_rotation;		//2D so we only need a single float to represent our rotation
	float		m_angularVelocity;
	float		m_angularDrag;
	const float MIN_ANGULAR_THRESHOLD;
	
	//Material
	PhysicsMaterial*	m_material;

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
		bool isKinematic = false,
		PhysicsMaterial* material = new PhysicsMaterial(0.4f, 0.9f, 1000),
		float mass = 1.f,
		float moment = 1.f,
		float linearDrag = 0.05f,
		float angDrag = 0.15f,
		float minLinearThreshold = 0.1f,
		float minAngularThreshold = 0.01f);
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
	bool		isKinematic() const { return m_isKinematic; }

	//Drag
	float		getLinearDrag() const { return m_linearDrag; }
	void		setLinearDrag(const float lDrag) { m_linearDrag = std::clamp(lDrag, 0.0f, 1.0f); }
	float		getAngularDrag() const { return m_angularDrag; }
	void		setAngularDrag(const float angDrag) { m_angularDrag = std::clamp(angDrag, 0.0f, 1.0f); }

	//Material
	PhysicsMaterial*	getMaterial() const { return m_material; }
	void				setMaterial(PhysicsMaterial* material) { m_material = material; }

	//Restitution
	void		displace(const vec2& displacement);
};

