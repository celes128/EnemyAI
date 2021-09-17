#include "strat_revive.h"

#include "stratutils.h"

using su = stratutils;

Result strat_revive(IN ReadyEntityInfo &rei, OUT Action *action)
{
	assert(action);

	auto smartness = 1;
	auto targetTypes = su::gen_target_types_revive(smartness);

	for (auto &tarType : targetTypes) {
		auto result = su::choose_revive_action(tarType, rei, action);
		if (ok(result)) {
			return kSuccess;
		}
	}

	return kFailure;
}