#pragma once

#include "framework.h"

/*
	Entities have a list of SpellInstances.
	A SpellInstance is a spell learnt by an entity.
	The spell goes on cooldown when the entity casts it.
*/
class SpellInstance {
public:
	SpellInstance(uint spellId, uint cooldown);

	auto Id() const { return m_id; }
	auto RemainingCooldown() const { return m_remainingCooldown; }
	auto Ready() const { return 0 == m_remainingCooldown; }
	
	void TriggerCooldown() { m_remainingCooldown = m_originalCooldown; }

	void AdvanceCooldown(uint numTurns = 1);
private:
	uint		m_id;

	// Cooldowns are expressed in number of turns.
	uint		m_originalCooldown;
	uint		m_remainingCooldown;
};