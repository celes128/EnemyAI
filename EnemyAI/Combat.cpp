#include "Combat.h"

#include <cassert>

#include "CombatStateInitialize.h"
#include "CombatStateTerminate.h"
#include "CombatStateBeginTurn.h"
#include "CombatStateTurn.h"

#include "Entity.h"
#include "EventSystem.h"

// ------------------------------------------
//  -----------------            ICombatState
// ------------------------------------------
ICombatState::ICombatState(Combat *combat)
	:
	m_combat(combat)
{}

Combat::Combat(
	EventSystem *eventSystem,
	Bank<SpellData> *spellBank,
	Party *party1,
	Party *party2
)
	:
	m_eventSystem(eventSystem),
	m_spellBank(spellBank),
	m_parties{{party1, party2}},
	m_states(COMBATSTATE_COUNT, nullptr)
{
	create_all_states();

	transition_to_state(COMBATSTATE_INITIALIZE);
}

Combat::~Combat()
{
	for (auto &state : m_states) {
		if (state) {
			delete state;
			state = nullptr;
		}
	}
}


// ------------------------------------
//  -----------------            Combat
// ------------------------------------
void Combat::Update()
{
	m_states[m_currentState]->Update();
}

bool Combat::Over() const
{
	return m_currentState == COMBATSTATE_TERMINATE;
}

void Combat::create_all_states()
{
	m_states[COMBATSTATE_INITIALIZE] = new CombatStateInitialize(this);
	m_states[COMBATSTATE_TERMINATE] = new CombatStateTerminate(this);
	m_states[COMBATSTATE_BEGINTURN] = new CombatStateBeginTurn(this);
	m_states[COMBATSTATE_TURN] = new CombatStateTurn(this);
}

void Combat::transition_to_state(COMBATSTATE state)
{
	assert(0 <= state && state < COMBATSTATE_COUNT);

	m_currentState = state;
	
	assert(m_states[m_currentState]);
	m_states[m_currentState]->OnEnter();
}

bool Combat::a_party_is_dead() const
{
	for (auto &party : m_parties) {
		if (dead_party(party)) {
			return true;
		}
	}
	return false;
}

Party *Combat::opponents(Party *party)
{
	assert(m_parties[0] == party || m_parties[1] == party);

	return m_parties[0] == party ? m_parties[1] : m_parties[0];
}

Result Combat::find_entity_from_id(IN uint id, OUT Entity **entity)
{
	assert(entity);

	for (auto &party : m_parties) {
		for (size_t i = 0; i < party->size(); ++i) {
			auto &e = (*party)[i];
			if (e->Id() == id) {
				*entity = e;
				return kSuccess;
			}
		}
	}
	
	return kFailure;
}



bool dead_party(const Party *party)
{
	auto it = std::find_if(
		party->begin(),
		party->end(),
		[](const Entity *entity) { return entity->Alive(); }
	);

	return it == party->end();
}