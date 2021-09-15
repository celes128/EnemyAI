#include "CombatStateTurn.h"

#include "EventSystem.h"
#include "utils.h"

CombatStateTurn::CombatStateTurn(Combat *combat)
	:
	ICombatState(combat)
{}

void CombatStateTurn::OnEnter()
{
}

void CombatStateTurn::Update()
{
	ReadyEntity re;
	auto result = m_combat->m_readyList.Next(&re);
	
	if (ok(result)) {
		auto action = choose_action(re);

		process_action(action);
	}
	else {
		// The entire ready list has been cycled through
		//m_combat->transition_to_state(COMBATSTATE_ENDTURN);
		m_combat->transition_to_state(COMBATSTATE_TERMINATE);
	}
}

Action CombatStateTurn::choose_action(const ReadyEntity &re)
{
	Entity *entity;
	auto result = utils::party::entity_from_id(re.id, *re.party, &entity);
	assert(ok(result));

	auto *opponents = m_combat->opponents(re.party);

	return entity->ChooseAction(*re.party, *opponents);
}

void CombatStateTurn::process_action(Action action)
{
	using T = Action::Type;

	switch (action.type) {
		case T::SpellCast: process_action_spellcast(action); break;
		case T::None: process_action_none(action); break;
		default: assert(false); break;
	}
}

void CombatStateTurn::process_action_spellcast(Action action)
{
	assert(Action::Type::SpellCast == action.type);

	Entity *caster;
	auto result = m_combat->find_entity_from_id(IN action.caster, OUT &caster);
	if (failed(result))
		return;

	for (auto &id: action.targets) {
		Entity *target;
		result = m_combat->find_entity_from_id(IN id, OUT &target);
		if (failed(result))
			continue;

		auto ev = Event::MakeSpellCast(action.asSpellCast.spellId, caster, target);
		m_combat->m_eventSystem->PostEvent(ev);

		// TODO: process each spell effect
	}

	caster->TriggerSpellCooldown(action.asSpellCast.spellId);
}

void CombatStateTurn::process_action_none(Action action)
{
	assert(Action::Type::None == action.type);

	Entity *caster;
	auto result = m_combat->find_entity_from_id(IN action.caster, OUT &caster);
	if (failed(result))
		return;

	auto ev = Event::MakeActionNone(caster);
	m_combat->m_eventSystem->PostEvent(ev);
}