#pragma once

#include <vector>

#include "framework.h"
#include "Resource.h"
#include "MagicSchool.h"
#include "TargetingData.h"

struct SpellEffect {
	enum Type {
		None,

		ModifyResource
	};

	Type					type;
	MagicSchool				school;

	struct AsResource {
		Resource			resource;
		uint				potency;
	};

	union {
		AsResource			asResource;
	};

	static SpellEffect MakeModifyResource(Resource resource, uint potency, MagicSchool school = MagicSchool::None)
	{
		SpellEffect fx;

		fx.type = Type::ModifyResource;
		fx.school = school;

		fx.asResource.resource = resource;
		fx.asResource.potency = potency;

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