#include "PhysicsMaterial.h"


float PhysicsMaterial::getDensityFromMaterial(const eMaterial material)
{
	//All densities in kg.m-3
	switch (material)
	{
	case eMaterial::HYDROGEN:		return 0.0898f;			break;	//Lightest Gas
	case eMaterial::HELIUM:			return 0.179f;			break;
	case eMaterial::AEROGRAPHITE:	return 0.2f;			break;	//Lightest Solid
	case eMaterial::AIR:			return 1.2f;			break;
	case eMaterial::TUNGSTEN_HEXAFLOURIDE: return 12.4f;	break;	//Heaviest Gas
	case eMaterial::STYROFOAM:		return 75;				break;
	case eMaterial::CORK:			return 240;				break;
	case eMaterial::PINE:			return 373;				break;
	case eMaterial::LITHIUM:		return 535;				break;
	case eMaterial::WOOD:			return 700;				break;
	case eMaterial::ICE:			return 916.7f;			break;
	case eMaterial::WATER:			return 1000;			break;
	case eMaterial::SALT_WATER:		return 1030;			break;
	case eMaterial::PLASTICS:		return 1175;			break;
	case eMaterial::MAGNESIUM:		return 1740;			break;
	case eMaterial::CONCRETE:		return 2400;			break;
	case eMaterial::DIAMOND:		return 3500;			break;
	case eMaterial::VANADIUM:		return 6100;			break;
	case eMaterial::BRASS:			return 8600;			break;
	case eMaterial::COPPER:			return 8940;			break;
	case eMaterial::SILVER:			return 10500;			break;
	case eMaterial::LEAD:			return 11340;			break;
	case eMaterial::RHODIUM:		return 12410;			break;
	case eMaterial::MERCURY:		return 13546;			break;	//Heaviest Liquid
	case eMaterial::TUNGSTEN:		return 19300;			break;
	case eMaterial::GOLD:			return 19320;			break;
	case eMaterial::PLATINUM:		return 21450;			break;
	case eMaterial::IRIDIUM:		return 22420;			break;
	case eMaterial::OSMIUM:			return 22570;			break;	//Heaviest Solid
	default:						return -1;
	}
}

PhysicsMaterial::PhysicsMaterial(float friction, float elasticity, float density) :
	friction(friction), elasticity(elasticity), m_density(density)
{
	m_material = eMaterial::NIL;
}

PhysicsMaterial::PhysicsMaterial(float friction, float elasticity, eMaterial material) :
	friction(friction), elasticity(elasticity), m_material(material)
{
	m_density = getDensityFromMaterial(material) /** 0.0001f*/;		//Divide by 1000 to convert to kg/cm3 (msi units)
}

void PhysicsMaterial::setMaterial(const eMaterial material)
{
	m_material = material;
	m_density = getDensityFromMaterial(material) /** 0.0001f*/;		//Divide by 1000 to convert to kg/cm3 (msi units)
}

eMaterial PhysicsMaterial::getMaterial() const
{
	return m_material;
}

void PhysicsMaterial::setDensity(const float density)
{
	//Custom density value will unset material (cheap method)
	m_material = eMaterial::NIL;
	m_density = density;
}
