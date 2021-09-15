#include "utils.h"

#include <algorithm>

#include "Entity.h"

namespace utils {

	namespace party {

		Result entity_from_id(IN uint id, IN Party &party, OUT Entity **entity)
		{
			assert(entity);

			auto it = std::find_if(
				party.begin(),
				party.end(),
				[id](const Entity *e) { return e->Id() == id; }
			);
			if (it != party.end()) {
				*entity = *it;
				return kSuccess;
			}

			return kFailure;
		}
	}
}