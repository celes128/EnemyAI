#include "CombatStateEndTurn.h"

#include "EventSystem.h"

CombatStateEndTurn::CombatStateEndTurn(Combat *combat)
	:
	ICombatState(combat)
{}

void CombatStateEndTurn::OnEnter()
{
	m_combat->m_eventSystem->PostEvent(Event::MakeCombatTurnEnd());
}

void CombatStateEndTurn::Update()
{
	// TODO: tick buffs
	
	m_combat->transition_to_state(COMBATSTATE_BEGINTURN);
}