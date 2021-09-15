#include "CombatStateTerminate.h"

#include "EventSystem.h"

CombatStateTerminate::CombatStateTerminate(Combat *combat)
	:
	ICombatState(combat)
{}

void CombatStateTerminate::OnEnter()
{
	m_combat->m_eventSystem->PostEvent(Event::MakeCombatEnd());
}

void CombatStateTerminate::Update()
{
}