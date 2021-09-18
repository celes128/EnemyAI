#include "strat_offensive.h"

#include "stratutils.h"

Result strat_offensive(IN ReadyEntityInfo &rei, OUT Action *action)
{
	assert(action);

	auto smartness = 1;
	auto targetTypes = stratutils::gen_target_types_damage(smartness);

	for (auto &tarType : targetTypes) {
		auto result = stratutils::choose_offensive_action(tarType, rei, action);
		if (ok(result)) {
			return kSuccess;
		}
	}

	return kFailure;
}