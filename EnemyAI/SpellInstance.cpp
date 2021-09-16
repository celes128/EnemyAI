#include "SpellInstance.h"

#include <algorithm>

SpellInstance::SpellInstance(uint spellId)
	:
	m_id(spellId),
	m_remainingCooldown(0)
{}

void SpellInstance::AdvanceCooldown(uint numTurns)
{
	if (numTurns > m_remainingCooldown) {
		m_remainingCooldown = 0;
	}
	else {
		m_remainingCooldown -= numTurns;
	}
}