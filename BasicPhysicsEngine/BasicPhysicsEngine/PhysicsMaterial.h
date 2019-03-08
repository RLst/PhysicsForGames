#pragma once

enum class eMaterial
{
	NIL,
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
{	//Super basic physics material class
private:
	eMaterial	m_material;
	float		m_density = 1000;

private:
	float		getDensityFromMaterial(const eMaterial material);

public:
	float		friction = 0.4f;
	float		elasticity = 0.9f;

	PhysicsMaterial(float friction, float elasticity, float density = 1000);
	PhysicsMaterial(float friction, float elasticity, eMaterial material);
	~PhysicsMaterial() {}

	void		setMaterial(const eMaterial material);
	eMaterial	getMaterial() const;

	void		setDensity(const float density);
	float		getDensity() const { return m_density; }
};