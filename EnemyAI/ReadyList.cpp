#include "ReadyList.h"

#include <cassert>

ReadyList::ReadyList()
{}

ReadyList::ReadyList(std::vector<Party *> parties)
{
	for (auto &party : parties) {
		for (auto &entity : *party) {
			if (entity->Alive()) {
				m_entities.push(ReadyEntity{ entity->Id(), party });
			}
		}
	}
}

Result ReadyList::Next(ReadyEntity *re)
{
	if (!Empty()) {
		*re = m_entities.front();
		m_entities.pop();

		return kSuccess;
	}
	return kFailure;
}