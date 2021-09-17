#include "stratutils.h"

#include "TargetingData.h"
#include "Action.h"
#include "Entity.h"
#include "Query.h"

static Bank<SpellData> *s_spellbank;

static bool offensive_spell(const SpellData *spell)
{
	for (auto &effect : spell->effects) {
		if (SpellEffect::Type::ModifyResource == effect.type) {
			switch (effect.asResource.modifType) {
			case RESOURCE_MODIF_TYPE_DAMAGE:
			case RESOURCE_MODIF_TYPE_STEAL:
			case RESOURCE_MODIF_TYPE_EXPLODE:
				return true;
				break;

			default:
				break;
			}
		}
	}

	return false;
}

static bool revive_spell(const SpellData *spell)
{
	for (auto &effect : spell->effects) {
		if (SpellEffect::Type::Revive == effect.type) {
			return true;
		}
	}

	return false;
}

Result stratutils::Initialize(Bank<SpellData> *spellbank)
{
	s_spellbank = spellbank;

	return kSuccess;
}

void stratutils::Quit()
{}



std::vector<const SpellData *> stratutils::spell_datas_from_ids(const std::vector<uint> &spellIds)
{
	std::vector<const SpellData *> out;

	for (auto id : spellIds) {
		// Retrieve spell data from the bank
		const SpellData *spellData;
		auto result = s_spellbank->Find(id, &spellData);
		if (ok(result)) {
			out.push_back(spellData);
		}
	}

	return out;
}

std::array<int, 4> stratutils::gen_target_types_damage(int smartness)
{
	if (smartness >= 3) {
		return { fTarParty, fTarEntity, fTarEverybody, fTarSelf };
	}
	else {
		return { fTarEntity, fTarParty, fTarEverybody, fTarSelf };
	}
}

std::array<int, 4> stratutils::gen_target_types_revive(int smartness)
{
	return gen_target_types_damage(smartness);
}

Result stratutils::choose_offensive_action(IN int targetType, IN ReadyEntityInfo &rei, OUT Action *action)
{
	switch (targetType) {
	case fTarEntity: return choose_offensive_action_entity(rei, action); break;

	default: assert(false && "Unknown target type"); break;
	}

	return kFailure;
}

Result stratutils::choose_revive_action(IN int targetType, IN ReadyEntityInfo &rei, OUT Action *action)
{
	switch (targetType) {
	case fTarEntity: return choose_revive_action_entity(rei, action); break;

	default: break;
	}

	return kFailure;
}

Result stratutils::choose_offensive_action_entity(IN ReadyEntityInfo &rei, OUT Action *action)
{
	Result result;

	// Choosing the spell
	auto readySpells = ready_spells(rei.entity);

	auto spells = filter_spells(
		readySpells,
		[](const SpellData *spell) { return spell->targetingData.Flags(fTarEntity); }
	);

	spells = filter_spells(
		spells,
		[](const SpellData *spell) { return offensive_spell(spell); }
	);

	size_t i;
	result = Query::RandomElement(IN spells, OUT &i);
	if (failed(result)) {
		return kFailure;
	}
	const auto kChosenSpellId = spells[i]->id;

	// Choosing a target
	Entity *target;
	result = Query::AnyAliveEntity(IN rei.opponents, OUT &target);
	if (failed(result)) {
		return kFailure;
	}
	const std::vector<uint> kChosenTargetIds{ {target->Id()} };

	// Success!
	*action = Action::MakeSpellCast(kChosenSpellId, rei.entity.Id(), kChosenTargetIds);
	return kSuccess;
}

Result stratutils::choose_revive_action_entity(IN ReadyEntityInfo &rei, OUT Action *action)
{
	Result result;

	// Choosing the spell
	auto spells = ready_spells(rei.entity);

	spells = filter_spells(
		spells,
		[](const SpellData *spell) { return spell->targetingData.Flags(fTarEntity); }
	);

	spells = filter_spells(
		spells,
		[](const SpellData *spell) { return revive_spell(spell); }
	);

	size_t i;
	result = Query::RandomElement(IN spells, OUT &i);
	if (failed(result)) {
		return kFailure;
	}
	const auto kChosenSpellId = spells[i]->id;

	// Choosing a target
	Entity *target;
	result = Query::AnyDeadEntity(IN rei.allies, OUT &target);
	if (failed(result)) {
		return kFailure;
	}
	const std::vector<uint> kChosenTargetIds{ {target->Id()} };

	// Success!
	*action = Action::MakeSpellCast(kChosenSpellId, rei.entity.Id(), kChosenTargetIds);
	return kSuccess;
}

std::vector<const SpellData *> stratutils::ready_spells(Entity &entity)
{
	return spell_datas_from_ids(entity.ReadySpells());
}
