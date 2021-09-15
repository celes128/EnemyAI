#include "CombatStateInitialize.h"

#include "EventSystem.h"

CombatStateInitialize::CombatStateInitialize(Combat *combat)
	:
	ICombatState(combat)
{}

void CombatStateInitialize::OnEnter()
{
	// TODO: initialize stuff
	// ...

	m_combat->m_eventSystem->PostEvent(Event::MakeCombatBegin());
}

void CombatStateInitialize::Update()
{
	m_combat->transition_to_state(COMBATSTATE_BEGINTURN);
}