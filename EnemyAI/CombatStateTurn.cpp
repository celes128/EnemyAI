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
	else {// The entire ready list has been cycled through
		m_combat->transition_to_state(COMBATSTATE_ENDTURN);
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

	Result result;

	// Get the spell data from the bank
	const SpellData *spell;
	result = m_combat->m_spellBank->Find(IN action.asSpellCast.spellId, &spell);
	if (failed(result)) {
		return;
	}

	// Get the caster
	Entity *caster;
	result = m_combat->find_entity_from_id(IN action.caster, OUT &caster);
	if (failed(result)) {
		return;
	}

	// Get the targets
	std::vector<Entity *> targets;
	for (auto &id : action.targets) {
		Entity *target;
		result = m_combat->find_entity_from_id(IN id, OUT &target);
		if (ok(result)) {
			targets.push_back(target);
		}
	}

	process_spell(*spell, caster, targets);

	caster->TriggerSpellCooldown(action.asSpellCast.spellId, spell->cooldown);
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

void CombatStateTurn::process_spell(const SpellData &spell, Entity *caster, std::vector<Entity *> &targets)
{
	for (auto &effect : spell.effects) {
		for (auto &target : targets) {
			process_spell_effect(effect, caster, target);
		}
	}

	for (auto &target : targets) {
		m_combat->m_eventSystem->PostEvent(Event::MakeSpellCast(spell.id, caster, target));
	}
}

void CombatStateTurn::process_spell_effect(const SpellEffect &effect, Entity *caster, Entity *target)
{
	using T = SpellEffect::Type;

	switch (effect.type) {
	case T::ModifyResource: process_spell_effect_resource(effect, caster, target); break;
	case T::None: break;
	}
}

void CombatStateTurn::process_spell_effect_resource(const SpellEffect &effect, Entity *caster, Entity *target)
{
	assert(SpellEffect::Type::ModifyResource == effect.type);

	const auto &fx = effect.asResource;

	switch (fx.resource) {
	case Resource::HP: {
		// TODO:
		//	Replace this identity function by one that takes into account
		//	the caster and the target attributes: spell power, attack power, armor, magic resistance...
		int amount = static_cast<int>(fx.potency);

		if (RESOURCE_MODIF_TYPE_DAMAGE == fx.modifType) {
			if (amount > 0) {
				amount *= -1;
			}
		}
		else if (RESOURCE_MODIF_TYPE_HEAL == fx.modifType) {
			if (amount < 0) {
				amount *= -1;
			}
		}
		else if (RESOURCE_MODIF_TYPE_STEAL == fx.modifType) {
			// TODO
		}
		else if (RESOURCE_MODIF_TYPE_EXPLODE == fx.modifType) {
			// TODO
		}
		else {
			assert(false && "Unknown RESOURCE_MODIF_TYPE");
		}

		int before, after;
		target->AddHP(amount, HEALING_EXTRAINFO_DOES_NOT_REVIVE, &before, &after);

		auto variation = after - before;
		if (variation != 0) {
			m_combat->m_eventSystem->PostEvent(Event::MakeModifHP(variation, caster, target));
		}

		// Check for death
		if (0 == after && variation < 0) {
			m_combat->m_eventSystem->PostEvent(Event::MakeDeath(target, caster, -variation));
		}
	}break;

	case Resource::MP: {
		// TODO
	}break;

	default:
		break;
	}
}