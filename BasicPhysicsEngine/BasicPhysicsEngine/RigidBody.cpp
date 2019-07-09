#include "RigidBody.h"

#include <iostream>

void RigidBody::displace(const vec2& displacement)
{
	if (m_isKinematic) return;

	m_position += displacement;
}

RigidBody::RigidBody(eShapeType shapeID,
	const vec2& position,
	const vec2& velocity,
	float rotation, 
	bool isKinematic,
	PhysicsMaterial* material,
	float mass,
	float moment,
	float linearDrag,
	float angularDrag,
	float minLinearThreshold,
	float minAngularThreshold) :
	PhysicsObject(shapeID),	//Avoids PhysicsObject requiring a default ctr
	m_position(position),
	m_velocity(velocity),
	m_rotation(rotation),
	m_mass(mass),
	m_moment(moment),
	m_material(material),
	m_isKinematic(isKinematic),
	MIN_LINEAR_THRESHOLD(minLinearThreshold),
	MIN_ANGULAR_THRESHOLD(minAngularThreshold)
{
	setLinearDrag(linearDrag);
	setAngularDrag(angularDrag);
}

RigidBody::~RigidBody()
{
	if (m_material != nullptr)
		delete m_material;
}

void RigidBody::FixedUpdate(const vec2& gravity, float timeStep)
{
	//Stop if kinematic
	if (m_isKinematic) return;

	ApplyForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;
	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	//Increase stability by stopping movement if velocities are low
	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = vec2(0, 0);
	}
	if (fabsf(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
	{
		m_angularVelocity = 0;
	}

	Debug();
}

void RigidBody::Debug()
{
#ifdef _DEBUG
	//system("cls");
	//std::cout << "ObjID: " << m_shapeID << std::endl;
	//std::cout << "mass: " << m_mass << std::endl;
	//std::cout << "x: " << m_position.x << ", y: " << m_position.y << std::endl;
	//std::cout << "vx: " << m_velocity.x << ", vy: " << m_velocity.y << std::endl;
	//std::cout << "ang: " << m_rotation << std::endl;
#endif
}

void RigidBody::ApplyForce(const vec2& newForce)
{
	//Newton's second law
	//(a = F / m, v = v + a)
	m_velocity += newForce / m_mass;	//Don't need dt as it's a fixed timestep
}

void RigidBody::ApplyForceToActor(RigidBody * actor2, const vec2& newForce)
{
	//Apply input force to the input actor (using ApplyForce of that actor)
	actor2->ApplyForce(newForce);

	//Apply opposite force to the current actor (by applying negative force)
	this->ApplyForce(-newForce);
}

void RigidBody::ResolveCollision(RigidBody * other)
{
	vec2 normal = glm::normalize(other->position() - m_position);
	vec2 relVelocity = other->velocity() - m_velocity;

	//TODO Need to make a physics material class/struct?
	float resultantElasticity = (m_material->elasticity + other->getMaterial()->elasticity) / 2.0f;
	
	//Super formula (impulse magnitude)
	float j = glm::dot(-(1 + resultantElasticity) * (relVelocity), normal) / glm::dot(normal, normal * ((1 / m_mass) + (1 / other->mass())));

	vec2 force = normal * j;

	ApplyForceToActor(other, force);
}