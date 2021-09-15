#include "CombatStateBeginTurn.h"

#include "EventSystem.h"

CombatStateBeginTurn::CombatStateBeginTurn(Combat *combat)
	:
	ICombatState(combat)
{}

void CombatStateBeginTurn::OnEnter()
{
	m_combat->m_eventSystem->PostEvent(Event::MakeCombatTurnBegin());
}

void CombatStateBeginTurn::Update()
{
	if (m_combat->a_party_is_dead()) {
		m_combat->transition_to_state(COMBATSTATE_TERMINATE);
	}
	else {
		m_combat->m_readyList = ReadyList(m_combat->m_parties);

		m_combat->transition_to_state(COMBATSTATE_TURN);
	}
}