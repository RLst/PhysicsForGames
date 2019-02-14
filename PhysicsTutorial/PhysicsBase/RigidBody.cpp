#include "RigidBody.h"

#include <iostream>

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) :
	PhysicsObject(shapeID),	//Avoids PhysicsObject requiring a default ctr
	m_position(position),
	m_velocity(velocity),
	m_rotation(rotation),
	m_mass(mass)
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	ApplyForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;
}

void RigidBody::Debug()
{
#ifdef _DEBUG
	std::cout << "ObjID: " << m_shapeID << std::endl;
	std::cout << "mass: " << m_mass << std::endl;
	std::cout << "x: " << m_position.x << ", y: " << m_position.y << std::endl;
	std::cout << "vx: " << m_velocity.x << ", vy: " << m_velocity.y << std::endl;
	std::cout << "ang: " << m_rotation << std::endl;
#endif
}

void RigidBody::ApplyForce(glm::vec2 newForce)
{
	//Newton's second law
	//(a = F / m, v = v + a)
	m_velocity += newForce / m_mass;	//Don't need dt as it's a fixed timestep
}

void RigidBody::ApplyForceToActor(RigidBody * actor2, glm::vec2 newForce)
{
	//Apply input force to the input actor (using ApplyForce of that actor)
	actor2->ApplyForce(newForce);

	//Apply opposite force to the current actor (by applying negative force)
	this->ApplyForce(-newForce);
}

void RigidBody::ResolveCollision(RigidBody * other)
{
	glm::vec2 normal = glm::normalize(other->getPosition() - m_position);
	glm::vec2 relVelocity = other->getVelocity() - m_velocity;

	//Super formula (impulse magnitude)
	float elasticity = 0.99f;		//TODO

	float j = glm::dot(-(1 + elasticity) * (relVelocity), normal) / glm::dot(normal, normal * ((1 / m_mass) + (1 / other->getMass())));

	glm::vec2 force = normal * j;

	ApplyForceToActor(other, force);
}