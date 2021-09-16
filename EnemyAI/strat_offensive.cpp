#include "strat_offensive.h"

#include "stratutils.h"

using su = stratutils;

Result strat_offensive(IN ReadyEntityInfo &rei, OUT Action *action)
{
	assert(action);

	auto smartness = 1;
	auto targetTypes = su::gen_target_types_damage(smartness);

	for (auto &tarType : targetTypes) {
		auto result = su::choose_offensive_action(tarType, rei, action);
		if (ok(result)) {
			return kSuccess;
		}
	}

	return kFailure;
}