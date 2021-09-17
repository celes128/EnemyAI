#pragma once

#include <array>
#include <vector>

#include "framework.h"
#include "Bank.h"
#include "Spell.h"
#include "Party.h"

struct SpellData;
struct Action;
class Entity;

struct ReadyEntityInfo {
	Entity		&entity;
	Party		&allies;
	Party		&opponents;
};

//using StrategyFunc = Result(*)(IN ReadyEntityInfo &rei, OUT Action *action);

struct stratutils {
	stratutils() = delete;

	static Result Initialize(Bank<SpellData> *spellbank);
	static void Quit();

	static std::vector<const SpellData *> spell_datas_from_ids(const std::vector<uint> &spellIds);

	static std::array<int, 4> gen_target_types_damage(int smartness);
	static std::array<int, 4> gen_target_types_revive(int smartness);

	static Result choose_offensive_action(IN int targetType, IN ReadyEntityInfo &rei, OUT Action *action);
	static Result choose_revive_action(IN int targetType, IN ReadyEntityInfo &rei, OUT Action *action);

	// Try to find a damaging spell that targets one entity
	static Result choose_offensive_action_entity(IN ReadyEntityInfo &rei, OUT Action *action);
	// Try to find a revive spell that targets one entity
	static Result choose_revive_action_entity(IN ReadyEntityInfo &rei, OUT Action *action);

	static std::vector<const SpellData *> ready_spells(Entity &entity);


	template <class UnaryPredicate>
	static std::vector<const SpellData *> filter_spells(
		const std::vector<const SpellData *> &spells,
		UnaryPredicate p
	)
	{
		std::vector<const SpellData *> out;

		for (auto &spell : spells) {
			if (p(spell)) {
				out.push_back(spell);
			}
		}

		return out;
	}
};