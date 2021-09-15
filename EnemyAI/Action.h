#pragma once

#include <vector>

#include "framework.h"

struct Action {
	enum Type {
		None,		// Action that does nothing; ignored when processed by the engine
		
		SpellCast
	};

	Type type;// Discriminant

	// We use entity ids for caster and targets.
	uint					caster;
	std::vector<uint>		targets;

	struct AsSpellCast {
		uint		spellId;
	};

	union {
		AsSpellCast asSpellCast;
	};


	static Action MakeSpellCast(uint spellId, uint caster, std::vector<uint> targets)
	{
		Action ac;

		ac.type = Type::SpellCast;
		ac.caster = caster;
		ac.targets = targets;

		ac.asSpellCast.spellId = spellId;

		return ac;
	}

	static Action MakeNone(uint caster)
	{
		Action ac;

		ac.type = Type::None;

		ac.caster = caster;

		return ac;
	}

	Action() : type(Type::None)
	{}
};