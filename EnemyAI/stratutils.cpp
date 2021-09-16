#include "stratutils.h"

#include "TargetingData.h"
#include "Action.h"
#include "Entity.h"
#include "Query.h"

static Bank<SpellData> *s_spellbank;

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

Result stratutils::choose_offensive_action(IN int targetType, IN ReadyEntityInfo &rei, OUT Action *action)
{
	switch (targetType) {
	case fTarEntity:
		return choose_offensive_action_entity(rei, action);
		break;

	default:
		//assert(false && "Unknown target type");
		break;
	}

	return kFailure;
}

Result stratutils::choose_offensive_action_entity(IN ReadyEntityInfo &rei, OUT Action *action)
{
	Result result;

	// Choosing the spell
	auto readySpells = ready_spells(rei.entity);

	auto candidateSpells = filter_spells(
		readySpells,
		[](const SpellData *spell) { return spell->targetingData.Flags(fTarEntity); }
	);

	size_t i;
	result = Query::RandomElement(IN candidateSpells, OUT &i);
	if (failed(result)) {
		return kFailure;
	}
	const auto kChosenSpellId = candidateSpells[i]->id;

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

std::vector<const SpellData *> stratutils::ready_spells(Entity &entity)
{
	return spell_datas_from_ids(entity.ReadySpells());
}
