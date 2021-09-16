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
	// TODO: 
	// tick_buffs();

	advance_spell_cooldowns();

	m_combat->transition_to_state(COMBATSTATE_BEGINTURN);
}

void CombatStateEndTurn::advance_spell_cooldowns()
{
	// TODO:
	//	I could send events when a spell becomes ready.

	for (auto &party : m_combat->m_parties) {
		for (auto &entity : *party) {
			if (entity->Alive()) {
				entity->AdvanceAllSpellCooldowns();
			}
		}
	}
}