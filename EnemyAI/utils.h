#pragma once

#include <vector>

#include "framework.h"
#include "Party.h"

namespace utils {
	
	namespace party {
		
		/*
			PRECONDITIONS
			* index != nullptr
			RETURN VALUE
			* kSuccess if there is an entity matching the id in the party else kFailure.
			* In case of success, the function returns a pointer to the entity with the OUT parameter.
		*/
		Result entity_from_id(IN uint id, IN Party &party, OUT Entity **entity);
	}
};