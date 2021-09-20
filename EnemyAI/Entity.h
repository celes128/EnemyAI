#pragma once

#include <string>
#include <vector>

#include "framework.h"
#include "Party.h"
#include "Action.h"
#include "CombatStrategy.h"
#include "AIData.h"
#include "Spell.h"
#include "SpellInstance.h"

/*
	DESCRIPTION
	Defines the behaviour of an incoming heal on a dead entity.
		* HEALING_EXTRAINFO_DOES_NOT_REVIVE
		The heal will not revive the entity e.g. the heal does nothing at all.
		* HEALING_EXTRAINFO_REVIVE
		The heal will revive the entity.
*/
enum HEALING_EXTRAINFO {
	HEALING_EXTRAINFO_DOES_NOT_REVIVE,
	HEALING_EXTRAINFO_REVIVE
};

class Entity {
public:
	Entity(
		const std::string &name,
		uint id,
		int maxHP,
		const AIData &aiData,
		const std::vector<SpellInstance> &spells
	);

	const std::string &Name() const { return m_name; }
	uint Id() const { return m_id; }
	int HP() const { return m_curHP; }
	int MaxHP() const { return m_maxHP; }
	bool Alive() const { return m_curHP > 0; }
	bool Dead() const { return !Alive(); }

	/*
		ReadySpells returns the ids of the spells that are not on cooldown.
	*/
	std::vector<uint> ReadySpells() const;

	/*
		AddHP modifies the current HP of the entity.

		If amount > 0 the entity is healed depending on the value of healExtraInfo.
		If amount < 0 the entity is damaged unless it is already dead.

		The caller can pass pointers to int to obtain the amount of HP before or after 
		the function modifies the entity's HP.
	*/
	void AddHP(
		int amount,
		HEALING_EXTRAINFO healExtraInfo = HEALING_EXTRAINFO_DOES_NOT_REVIVE,
		int *before = nullptr,
		int *after = nullptr
	);

	void TriggerSpellCooldown(uint spell, uint cooldown);

	void AdvanceAllSpellCooldowns();

	Action ChooseAction(Party &allies, Party &opponents);

private:
	void damage(int amount);
	void heal(int amount, HEALING_EXTRAINFO healExtraInfo);

	void on_death();
	void on_revive();

private:
	std::string					m_name;
	uint						m_id;
	
	int							m_maxHP;
	int							m_curHP;

	AIData						m_aiData;

	std::vector<SpellInstance>	m_spellInstances;
};

void kill_entity(Entity *entity);