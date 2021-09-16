#include "CombatStrategy.h"

#include "stratutils.h"
#include "strat_offensive.h"

static Bank<SpellData> *s_spellbank;

static bool valid_strategy(STRATEGYNAME strat)
{
	return 0 <= strat && strat < STRATEGYNAME_COUNT;
}

using StrategyFunc = Result(*)(IN ReadyEntityInfo &rei, OUT Action *action);

static StrategyFunc s_strategies[STRATEGYNAME_COUNT] =
{
	strat_offensive,
};

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