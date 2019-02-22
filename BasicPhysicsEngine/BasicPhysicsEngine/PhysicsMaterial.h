#pragma once


class PhysicsMaterial
{	//Super basic physics material class
public:
	float friction = 0.4f;
	float elasticity = 0.9f;
	PhysicsMaterial() = default;
	PhysicsMaterial(float friction, float elasticity) : friction(friction), elasticity(elasticity) {}
	~PhysicsMaterial();
};

