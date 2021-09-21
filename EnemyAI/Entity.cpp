#define NOMINMAX
#include "Entity.h"

#include <algorithm>

#include "CombatStrategy.h"

Entity::Entity(
	const std::string &name,
	uint id,
	int maxHP,
	const AIData &aiData,
	const std::vector<SpellInstance> &spells
)
	:
	m_name(name),
	m_id(id),
	m_maxHP(std::max(maxHP, 1)),
	m_curHP(m_maxHP),
	m_aiData(aiData),
	m_spellInstances(spells)
{}

void Entity::AddHP(int amount, HEALING_EXTRAINFO healExtraInfo, int *before, int *after)
{
	if (before)
		*before = m_curHP;

	if (amount < 0)
		damage(-amount);
	else if (amount > 0)
		heal_or_revive(amount, healExtraInfo);

	if (after)
		*after = m_curHP;
}

void Entity::TriggerSpellCooldown(uint spell, uint cooldown)
{
	auto it = std::find_if(
		m_spellInstances.begin(),
		m_spellInstances.end(),
		[spell](const SpellInstance cur) { return cur.Id() == spell; }
	);
	if (it != m_spellInstances.end()) {
		it->TriggerCooldown(cooldown);
	}
}

void Entity::AdvanceAllSpellCooldowns()
{
	for (auto &spell : m_spellInstances) {
		spell.AdvanceCooldown();
	}
}

std::vector<uint> Entity::ReadySpells() const
{
	std::vector<uint> spellIds;

	for (auto &spell : m_spellInstances) {
		if (spell.Ready()) {
			spellIds.push_back(spell.Id());
		}
	}

	return spellIds;
}

Action Entity::ChooseAction(Party &allies, Party &opponents)
{
	Action action;

	for (auto &strat : m_aiData.strategies) {
		auto result = strat::ChooseAction(strat, *this, allies, opponents, &action);
		if (ok(result)) {
			return action;
		}
	}

	return Action::MakeNone(this->Id());
}

void Entity::damage(int amount)
{
	assert(amount > 0);

	if (Alive()) {
		m_curHP = std::max(0, m_curHP - amount);

		if (Dead()) {
			on_death();
		}
	}
}

void Entity::heal_or_revive(int amount, HEALING_EXTRAINFO healExtraInfo)
{
	assert(amount > 0);

	const bool wasDead = Dead();

	bool doHeal = true;
	if (Dead() && HEALING_EXTRAINFO_DOES_NOT_REVIVE == healExtraInfo) {
		doHeal = false;
	}

	if (doHeal) {
		m_curHP = std::min(m_maxHP, m_curHP + amount);

		if (wasDead) {
			on_revive();
		}
	}
}

void Entity::on_death()
{

}

void Entity::on_revive()
{

}

void kill_entity(Entity *entity)
{
	assert(entity);

	entity->AddHP(-entity->MaxHP());
}