#include "RigidBody.h"




RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) :
	m_position(position),
	m_velocity(velocity),
	m_rotation(rotation),
	m_mass(mass)
{
	m_shapeID = shapeID;
}

RigidBody::~RigidBody()
{
}

void RigidBody::FixedUpdate(glm::vec2 gravity, float timeStep)
{


}

void RigidBody::Debug()
{

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


