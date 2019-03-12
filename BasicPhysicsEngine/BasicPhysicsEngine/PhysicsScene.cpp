#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"

//std
#include <iostream>
#include <vector>
#include <list>

//local
#include "GameDefines.h"
#include "Plane.h"
#include "Circle.h"
#include "AABB.h"
#include "SAT.h"

PhysicsScene::PhysicsScene(float timeStep, vec2 gravity, bool collisionEnabled) :
	m_timeStep(timeStep),
	m_gravity(gravity),
	m_isCollisionEnabled(collisionEnabled)
{
}

PhysicsScene::~PhysicsScene()
{
	//Clean up m_actors (TODO Buggy)
	for (auto actor : m_actors)
		if (actor != nullptr)
			delete actor;
}

void PhysicsScene::AddActor(PhysicsObject * actor)
{
	m_actors.push_back(actor);
}

bool PhysicsScene::RemoveActor(PhysicsObject * targetActor)
{
	//Go through the vector of actors...
	for (int i = 0; i < m_actors.size(); ++i)
	{
		//If the matching actor is found...
		if (m_actors[i] == targetActor)
		{
			//TODO Delete pointer then erase from array?
			delete m_actors[i];
			m_actors.erase(m_actors.begin() + i);
			return true;
		}
	}
	return false;
}

void PhysicsScene::Update(float deltaTime)
{
	static std::list<PhysicsObject*> dirty;

	//Update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto actor : m_actors)
		{
			actor->FixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;

		CheckForCollisions();	//And also resolve collisions
	}
}

void PhysicsScene::UpdateGizmos()
{
	for (auto actor : m_actors)
	{
		actor->DrawGizmo();
	}
}

void PhysicsScene::DebugScene()
{
	//int count = 0;
	//for (auto actor : m_actors)
	//{
	//	std::cout << count << " : ";
	//	actor->Debug();
	//	++count;
	//}
}

//Setup the collision function array
static fn collisionFuncArray[] =
{
	PhysicsScene::Plane2Plane,	PhysicsScene::Plane2Circle,		PhysicsScene::Plane2AABB,	PhysicsScene::Plane2SAT,
	PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle,	PhysicsScene::Circle2AABB,	PhysicsScene::Circle2SAT,
	PhysicsScene::AABB2Plane,	PhysicsScene::AABB2Circle,		PhysicsScene::AABB2AABB,		PhysicsScene::AABB2SAT,
	PhysicsScene::SAT2Plane,	PhysicsScene::SAT2Circle,		PhysicsScene::SAT2AABB,		PhysicsScene::SAT2SAT,
};

void PhysicsScene::CheckForCollisions()
{
	//Need to check for collisions against all objects except this one
	for (int outer = 0; outer < m_actors.size() - 1; ++outer)
	{
		for (int inner = outer + 1; inner < m_actors.size(); ++inner)
		{

			//QUAD TREE OPTIMISAITONS HERE?

			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeID1 = (int)object1->GetShapeID();
			int shapeID2 = (int)object2->GetShapeID();

			//Using function pointers
			int functionIDX = (shapeID1 * (int)eShapeType::SHAPE_COUNT) + shapeID2;	//Auto select the correct function
			fn fpCollision = collisionFuncArray[functionIDX];
			if (fpCollision != nullptr)
			{
				//did a collision occur
				fpCollision(object1, object2);
			}
		}
	}
}

//bool PhysicsScene::Plane2Plane(N/A)

bool PhysicsScene::Plane2Circle(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Plane* plane = (Plane*)obj1;
	Circle* circle = (Circle*)obj2;

	//If we are successful then test for collision
	if (circle != nullptr && plane != nullptr)
	{
		float intersection = circle->radius() - plane->distanceTo(circle->position());

		//If there is any intersecion it means the objects have collided
		if (intersection > 0.0f)
		{
			//Move circle out from the surface to prevent sticking
			//(The plane is static so it is easier to move object out)
			circle->displace(plane->normal() * intersection);

			//Objects have collided
			plane->ResolveCollision(circle);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Plane2AABB(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try casting
	Plane* plane = (Plane*)obj1;
	AABB* aabb = (AABB*)obj2;

	//If successful then test for collisions
	if (plane != nullptr && aabb != nullptr)
	{
		//Brute force; Go through all vertices
		for (auto vertex : aabb->vertices())
		{
			//Does it makes more sense if an intersection occurs, that the number is positive?
			float intersection = -plane->distanceTo(vertex);

			if (intersection > 0.0f)
			{
				//Move aabb out from the surface to prevent sticking
				aabb->displace(plane->normal() * intersection);

				//Collision detected!
				plane->ResolveCollision(aabb);
				return true;
			}
		}
	}
	return false;
}

bool PhysicsScene::Plane2SAT(PhysicsObject *obj1, PhysicsObject *obj2)
{
	Plane *plane = (Plane*)obj1;
	SAT *sat = (SAT*)obj2;

	if (plane != nullptr && sat != nullptr)
	{
		for (auto vertex : sat->vertices())
		{
			float intersection = -plane->distanceTo(vertex);
			if (intersection > 0)
			{
				//There is collision!
				sat->displace(plane->normal() * intersection);
				plane->ResolveCollision(sat);
				return true;
			}
		}
	}
	return false;
}

bool PhysicsScene::Circle2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Re-route
	return Plane2Circle(obj2, obj1);
}

bool PhysicsScene::Circle2Circle(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try to cast object to circle and circle
	Circle *circle1 = (Circle*)(obj1);
	Circle *circle2 = (Circle*)(obj2);

	//If we are successful then test for collision
	if (circle1 != nullptr && circle2 != nullptr)
	{
		//OPTIMISED!!!
		float dx = circle2->position().x - circle1->position().x;
		float dy = circle2->position().y - circle1->position().y;
		float radii = circle1->radius() + circle2->radius();
		
		//DOT is basically a way to multipy vectors together ie. in this case squaring them
		float sqrDist = glm::dot(vec2(dx, dy), vec2(dx, dy));

		if (sqrDist < radii * radii)
		{
			//Restitution
			float dist = sqrtf(sqrDist);
			float overlap = dist - radii;
			auto displaceVector = circle2->position() - circle1->position();
			displaceVector = glm::normalize(displaceVector);
			//Displace by half each
			circle1->displace(displaceVector * overlap * 0.5f);
			circle2->displace(-displaceVector * overlap * 0.5f);

			//Resolve collision
			circle1->ResolveCollision(circle2);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Circle2AABB(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try to cast objects to their types
	Circle* circle = (Circle*)obj1;
	AABB *aabb = (AABB*)obj2;

	//If casts were successful...
	if (circle != nullptr && aabb != nullptr)
	{
		////Test for collisions
		//Use clamping trick (A = clamp(C, min, max)
		vec2 clampedDistance = glm::clamp(circle->position(), aabb->min(), aabb->max());
		vec2 V = clampedDistance - circle->position();
		float result = glm::dot(V, V);

		//If there is any intersecion it means the objects have collided
		if (result <= (circle->radius() * circle->radius()))
		{
			//Objects have collided
			circle->ResolveCollision(aabb);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Circle2SAT(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//Re-route
	return SAT2Circle(obj2, obj1);
}

bool PhysicsScene::AABB2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Re-route
	return Plane2AABB(obj2, obj1);
}

bool PhysicsScene::AABB2Circle(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Re-route
	return Circle2AABB(obj2, obj1);
}

bool PhysicsScene::AABB2AABB(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try casts
	AABB *aabb1 = (AABB*)(obj1);
	AABB *aabb2 = (AABB*)(obj2);

	//If cast successful..
	if (aabb1 != nullptr && aabb2 != nullptr)
	{
		//Find any overlap (collision)
		float xOverlap = pkr::min(aabb1->max().x, aabb2->max().x) - pkr::max(aabb1->min().x, aabb2->min().x);
		float yOverlap = pkr::min(aabb1->max().y, aabb2->max().y) - pkr::max(aabb1->min().y, aabb2->min().y);

		//If there is over lap then there is collision
		if (xOverlap > 0 && yOverlap > 0)
		{
			////AABBs have collided! 
			//Move out of each other's boundary 
			//TODO Not as straightforward as I initially thought
			//aabb1->displace(vec2(-xOverlap, -yOverlap) / 2.0f);
			//aabb2->displace(vec2(xOverlap, yOverlap) / 2.0f);
			//aabb1->ApplyForceToActor(aabb2, vec2(xOverlap, yOverlap) * aabb1->mass());		//CRAP

			//Resolve collision
			aabb1->ResolveCollision(aabb2);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::AABB2SAT(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Re-route
	return SAT2AABB(obj2, obj1);
}

bool PhysicsScene::SAT2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Re-route
	return Plane2SAT(obj2, obj1);
}

bool PhysicsScene::SAT2Circle(PhysicsObject * obj1, PhysicsObject * obj2)
{
	SAT* sat = (SAT*)obj1;
	Circle* circle = (Circle*)obj2;

	//If successful then test for collision
	if (sat != nullptr && circle != nullptr)
	{
		//MTV
		float smallestOverlap = INFINITY;
		vec2 smallestAxis;

		//Check collision between all sat's surface normals
		for (auto axis : sat->surfaceNormals())
			//for (int i = 0; i < sat->vertices().size(); ++i)
		{
			//Get sat's projection for this axis
			vec2 satProject = sat->projection(axis);

			//Get circle's projection for this axis
			//[NOTE: Get projection at circle's center FIRST then +/- radius
			float circleMinProject = glm::dot(axis, circle->position()) - circle->radius();
			float circleMaxProject = glm::dot(axis, circle->position()) + circle->radius();
			vec2 circleProject = vec2(circleMinProject, circleMaxProject);

			float o = FindOverlap(satProject, circleProject);
			if (o < 0) {
				//No collision, exit
				return false;
			}
			else if (o < smallestOverlap) {
				smallestOverlap = o;
				smallestAxis = axis;
			}
		}
		//Collision detected!

		//Use MTV to push out of collision interference
		vec2 mtv = glm::normalize(smallestAxis) * smallestOverlap;
		float totalMass = sat->mass() + circle->mass();
		//sat->ApplyForceToActor(circle, mtv * sat->mass() / totalMass);

		sat->displace(-mtv * sat->mass() / totalMass);
		circle->displace(mtv * circle->mass() / totalMass);		//This is still hacky and doesn't look right

		//Resolve
		sat->ResolveCollision(circle);
		return true;
	}
	return false;
}

bool PhysicsScene::SAT2AABB(PhysicsObject * obj1, PhysicsObject * obj2)
{
	SAT* sat = (SAT*)obj1;
	AABB* aabb = (AABB*)obj2;

	if (sat != nullptr && aabb != nullptr)
	{
		//MTV
		float smallestOverlap = INFINITY;
		vec2 smallestAxis;

		//Test SAT against AABB
		for (int i = 0; i < aabb->vertices().size(); ++i)
		{
			//Only 2 axes to test
			vec2 horizontalAxis = vec2(0, 1);
			vec2 verticalAxis = vec2(1, 0);
			
			//Horizontal
			auto aabbHProjection = vec2(aabb->min().y, aabb->max().y);
			auto satHProjection = sat->projection(horizontalAxis);
			auto o = FindOverlap(satHProjection, aabbHProjection);
			if (o < 0) {
				return false;	//No collision
			}
			else if (o < smallestOverlap) {
				smallestOverlap = o; smallestAxis = horizontalAxis;
			}

			//Vertical
			auto aabbVProjection = vec2(aabb->min().x, aabb->max().x);
			auto satVProjection = sat->projection(verticalAxis);
			o = FindOverlap(satVProjection, aabbVProjection);
			if (o < 0) {
				return false;	//No collision
			}
			else if (o < smallestOverlap) {
				smallestOverlap = o; smallestAxis = verticalAxis;
			}
		}

		//Test AABB against SAT
		vec2array satTestAxes = sat->surfaceNormals();
		for (auto axis : satTestAxes)
		{
			vec2 projectionSat = sat->projection(axis);
			vec2 projectionAABB = aabb->projection(axis);
			float o = FindOverlap(projectionSat, projectionAABB);
			
			if (o < 0) {
				return false;
			}
			else if (o < smallestOverlap) {
				smallestOverlap = o;
				smallestAxis = axis;
			}
		}

		//COLLISION DETECTED!
		//Minimum Translation Vector
		vec2 mtv = glm::normalize(smallestAxis) * smallestOverlap;

		//Resolve
		sat->ResolveCollision(aabb);
		return true;
	}
	return false;
}

bool PhysicsScene::SAT2SAT(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try casting first
	//[NOTE: Had to make eShapeTypes a enum class otherwise it conflicts with SAT class
	SAT* sat1 = (SAT*)obj1;
	SAT* sat2 = (SAT*)obj2;

	//If cast successful...
	if (sat1 != nullptr && sat2 != nullptr)
	{
		//Minimum Translation Vector
		float smallestOverlap = INFINITY;
		vec2 smallestAxis;

		//1. Get list of surface normals ie. possible axes of separations
		vec2array axes1 = sat1->surfaceNormals();
		vec2array axes2 = sat2->surfaceNormals();

		//Project each shape's hull onto each shape's axis and check for overlap
		for (auto axis : axes1)
		{
			vec2 projection1 = sat1->projection(axis);
			vec2 projection2 = sat2->projection(axis);

			//3. Determine if there's any overlap
			//If there's a negative overlap then there's definitely NO COLLISION
			float o = FindOverlap(projection1, projection2);
			if (o < 0) {
				//No collision, get out
				return false;
			}
			else if (o < smallestOverlap) {	//Grab the MTV too
				smallestOverlap = o;
				smallestAxis = axis;
			}
		}
		for (auto axis : axes2)
		{
			vec2 projection1 = sat1->projection(axis);
			vec2 projection2 = sat2->projection(axis);

			//3. Determine if there's any overlap
			//If there's a negative overlap then there's definitely NO COLLISION
			float o = FindOverlap(projection1, projection2);
			if (o < 0) {
				//No collision, get out
				return false;
			}
			else if (o < smallestOverlap) {	//Grab the MTV too
				smallestOverlap = o;
				smallestAxis = axis;
			}
		}
		//All axes overlapped so there is COLLISION!

		//Find the minimum translation vector so that we can resolve the collision
		vec2 mtv = glm::normalize(smallestAxis) * smallestOverlap;

		////Use MTV to move out of collision interference into collision clearance or transition
		//MTV is the collision overlap!
		//try it just on the first object
		//sat1->displace(mtv);
		
		//float totalMass = sat1->mass() + sat2->mass();
		//sat1->displace(-mtv * sat1->mass() / totalMass);
		//sat2->displace(mtv * sat2->mass() / totalMass);		//This is still hacky and doesn't look right

		sat1->ResolveCollision(sat2);
		return true;
	}
	return false;
}

float PhysicsScene::FindOverlap(const vec2 & projection1, const vec2 & projection2)
{
	//Find mins
	float a = projection1.y - projection2.x;
	float b = projection2.y - projection1.x;
	return a < b ? a : b;
}
