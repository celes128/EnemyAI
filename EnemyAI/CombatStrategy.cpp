#include "CombatStrategy.h"

#include <array>

#include "TargetingData.h"
#include "Action.h"
#include "Entity.h"
#include "Query.h"

static Bank<SpellData> *s_spellbank;

struct ReadyEntityInfo {
	Entity		&entity;
	Party		&allies;
	Party		&opponents;
};

static bool valid_strategy(STRATEGYNAME strat)
{
	return 0 <= strat && strat < STRATEGYNAME_COUNT;
}

using StrategyFunc = Result(*)(IN ReadyEntityInfo &rei, OUT Action *action);

static Result strat_offensive(IN ReadyEntityInfo &rei, OUT Action *action);

static StrategyFunc s_strategies[STRATEGYNAME_COUNT] =
{
	strat_offensive,
};

Result strat::Initialize(Bank<SpellData> *spellbank)
{
	assert(spellbank);

	s_spellbank = spellbank;

	return kSuccess;
}

void strat::Quit()
{}

Result strat::ChooseAction(
	IN STRATEGYNAME strat,
	IN Entity &entity,
	IN Party &allies,
	IN Party &opponents,
	OUT Action *action
)
{
	assert(valid_strategy(strat));

	ReadyEntityInfo rei{ entity, allies, opponents };
	return s_strategies[strat](rei, action);
}


// ------------------------------------------------------------------
//						  Utility functions
// ------------------------------------------------------------------

static std::vector<const SpellData *> spell_datas_from_ids(const std::vector<uint> &spellIds)
{
	std::vector<const SpellData *> out;

	for (auto id : spellIds) {
		// Retrieve spell data from the bank
		const SpellData *spellData;
		auto result = s_spellbank->Find(id, &spellData);
		if (ok(result)) {
			out.push_back(spellData);
		}
	}

	return out;
}

template <class UnaryPredicate>
static std::vector<const SpellData *>
filter_spells(const std::vector<const SpellData *> &spells, UnaryPredicate p)
{
	std::vector<const SpellData *> out;

	for (auto &spell : spells) {
		if (p(spell)) {
			out.push_back(spell);
		}
	}

	return out;
}

static std::vector<const SpellData *> ready_spells(Entity &entity)
{
	return spell_datas_from_ids(entity.ReadySpells());
}

// Try to find a damaging spell that targets one entity
static Result choose_offensive_action_entity(IN ReadyEntityInfo &rei, OUT Action *action)
{
	Result result;

	// Choosing the spell
	auto readySpells = ready_spells(rei.entity);

	auto candidateSpells = filter_spells(
		readySpells,
		[](const SpellData *spell) { return spell->targetingData.Flags(fTarEntity); }
	);

	size_t i;
	result = Query::RandomElement(IN candidateSpells, OUT &i);
	if (failed(result)) {
		return kFailure;
	}
	const auto kChosenSpellId = candidateSpells[i]->id;

	// Choosing a target
	Entity *target;
	result = Query::AnyAliveEntity(IN rei.opponents, OUT &target);
	if (failed(result)) {
		return kFailure;
	}
	const std::vector<uint> kChosenTargetIds{ {target->Id()} };

	// Success!
	*action = Action::MakeSpellCast(kChosenSpellId, rei.entity.Id(), kChosenTargetIds);
	return kSuccess;
}

static Result choose_offensive_action(IN int targetType, IN ReadyEntityInfo &rei, OUT Action *action)
{
	switch (targetType) {
	case fTarEntity:
		return choose_offensive_action_entity(rei, action);
		break;

	default:
		//assert(false && "Unknown target type");
		break;
	}

	return kFailure;
}

static std::array<int, 4> gen_target_types_damage(int smartness)
{
	if (smartness >= 3) {
		return { fTarParty, fTarEntity, fTarEverybody, fTarSelf };
	}
	else {
		return { fTarEntity, fTarParty, fTarEverybody, fTarSelf };
	}
}


// ------------------------------------------------------------------
//						The strategy functions
// ------------------------------------------------------------------
Result strat_offensive(IN ReadyEntityInfo &rei, OUT Action *action)
{
	assert(action);

	auto smartness = 1;
	auto targetTypes = gen_target_types_damage(smartness);

	for (auto &tarType : targetTypes) {
		auto result = choose_offensive_action(tarType, rei, action);
		if (ok(result)) {
			return kSuccess;
		}
	}

	return kFailure;
}