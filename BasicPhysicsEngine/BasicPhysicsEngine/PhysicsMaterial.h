#pragma once

enum class eMaterial
{
	NIL = -1,
	HYDROGEN,
	HELIUM,
	AEROGRAPHITE,
	AIR,
	TUNGSTEN_HEXAFLOURIDE,
	STYROFOAM,
	CORK,
	PINE,
	LITHIUM,
	WOOD,
	ICE,
	WATER,
	SALT_WATER,
	PLASTICS,
	MAGNESIUM,
	CONCRETE,
	DIAMOND,
	VANADIUM,
	BRASS,
	COPPER,
	SILVER,
	LEAD,
	RHODIUM,
	MERCURY,
	TUNGSTEN,
	GOLD,
	PLATINUM,
	IRIDIUM,
	OSMIUM,
	MATERIAL_COUNT
};

class PhysicsMaterial
{
private:
	eMaterial	m_material;
	float		m_density = 1000;

private:
	float		getDensityFromMaterial(const eMaterial material);

public:
	float		friction = 0.4f;
	float		elasticity = 0.9f;

public:
	//Construct with custom density
	PhysicsMaterial(float friction, float elasticity, float density);
	//Construct using material
	PhysicsMaterial(float friction, float elasticity, eMaterial material);
	~PhysicsMaterial() {}

	eMaterial	getMaterial() const;
	void		setMaterial(const eMaterial material);

	float		getDensity() const { return m_density; }
	void		setDensity(const float density);
};