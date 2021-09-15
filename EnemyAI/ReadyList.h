#pragma once

#include <queue>

#include "framework.h"
#include "Party.h"
#include "Entity.h"

struct ReadyEntity {
	uint				id;
	Party				*party;
};

class ReadyList {
public:
	ReadyList();
	ReadyList(std::vector<Party *> parties);

	bool Empty() const
	{
		return m_entities.empty();
	}

	/*
		RETURN VALUE
		* kSuccess if the list was not empty else kFailure.
	*/
	Result Next(ReadyEntity *re);

private:
	std::queue<ReadyEntity>	m_entities;
};