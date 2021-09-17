#pragma once

#include <vector>

#include "framework.h"
#include "Resource.h"
#include "MagicSchool.h"
#include "TargetingData.h"

enum RESOURCE_MODIF_TYPE {
	RESOURCE_MODIF_TYPE_DAMAGE,
	RESOURCE_MODIF_TYPE_HEAL,
	RESOURCE_MODIF_TYPE_STEAL,
	RESOURCE_MODIF_TYPE_EXPLODE
};

struct SpellEffect {
	enum Type {
		None,

		ModifyResource,		// damage or restore either HP or MP
		Revive				// raise an entity back to life
	};

	Type					type;
	MagicSchool				school;

	struct AsResource {
		Resource			resource;
		uint				potency;
		RESOURCE_MODIF_TYPE	modifType;
	};

	struct AsRevive {
		uint				potency;
	};

	union {
		AsResource			asResource;
		AsRevive			asRevive;
	};

	static SpellEffect MakeModifyResource(Resource resource, uint potency, RESOURCE_MODIF_TYPE modifType, MagicSchool school = MagicSchool::None)
	{
		SpellEffect fx;

		fx.type = Type::ModifyResource;
		fx.school = school;

		fx.asResource.resource = resource;
		fx.asResource.potency = potency;
		fx.asResource.modifType = modifType;

		return fx;
	}

	static SpellEffect MakeRevive(uint potency, MagicSchool school = MagicSchool::None)
	{
		SpellEffect fx;

		fx.type = Type::Revive;
		fx.school = school;

		fx.asRevive.potency = potency;

		return fx;
	}

	SpellEffect() : type(Type::None)
	{}
};

struct SpellData {
	uint							id;
	std::vector<SpellEffect>		effects;
	TargetingData					targetingData;

	// Cooldowns are expressed in number of turns.
	uint							cooldown;
};