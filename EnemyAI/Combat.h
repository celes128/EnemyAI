#pragma once

#include <vector>

#include "ReadyList.h"
#include "Party.h"
#include "Bank.h"
#include "Spell.h"

class Entity;
class EventSystem;

class Combat;
class ICombatState {
public:
	ICombatState(Combat *combat);

	virtual void OnEnter() = 0;
	virtual void Update() = 0;

protected:
	Combat *m_combat;
};

enum COMBATSTATE {
	COMBATSTATE_INITIALIZE,
	COMBATSTATE_TERMINATE,
	COMBATSTATE_BEGINTURN,
	COMBATSTATE_TURN,
	COMBATSTATE_ENDTURN,

	COMBATSTATE_COUNT	// Number of states
};

class Combat {
public:
	friend class ICombatState;
	friend class CombatStateInitialize;
	friend class CombatStateTerminate;
	friend class CombatStateBeginTurn;
	friend class CombatStateTurn;
	friend class CombatStateEndTurn;

	Combat(
		EventSystem *eventSystem,
		Bank<SpellData> *spellBank,
		Party *party1,
		Party *party2
	);

	~Combat();

	void Update();

	bool Over() const;

private:
	void create_all_states();
	void transition_to_state(COMBATSTATE state);
	bool a_party_is_dead() const;

	/*
		PRECONDITIONS
		* party != nullptr
		* party is in the vector of parties (m_parties)
		RETURN VALUE
		Returns the opposing party.
	*/
	Party *opponents(Party *party);

	/*
		PRECONDITIONS
		* entity != nullptr
		RETURN VALUE
		Returns kSuccess if the id correspond to an entity from one of the parties else kFailure.
		In case of success, the function returns a pointer to the entity with the OUT parameter.
	*/
	Result find_entity_from_id(IN uint id, OUT Entity **entity);

private:
	EventSystem							*m_eventSystem;
	Bank<SpellData>						*m_spellBank;

	std::vector<Party *>				m_parties;

	std::vector<ICombatState *>			m_states;
	
	COMBATSTATE							m_currentState;

	ReadyList							m_readyList;
};

bool dead_party(const Party *party);