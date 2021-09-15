#pragma once

#include <vector>

#include "framework.h"
#include "Party.h"
#include "Bank.h"
#include "Spell.h"
#include "StrateyName.h"

struct Action;

struct strat {
	strat() = delete;

	static Result ChooseAction(
		IN STRATEGYNAME strat,
		IN Entity &entity,
		IN Party &allies,
		IN Party &opponents,
		OUT Action *action
	);
};