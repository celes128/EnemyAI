#include "Query.h"

#include "Entity.h"

std::default_random_engine Query::s_randEngine;

Result Query::AnyAliveEntity(IN Party &party, OUT Entity **entity)
{
	return RandomEntityIf(
		party,
		[](const Entity *e) { return e->Alive(); },
		entity
	);
}

Result Query::AnyDeadEntity(IN Party &party, OUT Entity **entity)
{
	return RandomEntityIf(
		party,
		[](const Entity *e) { return e->Dead(); },
		entity
	);
}