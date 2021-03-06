#include <iostream>
#include <memory>

#include "framework.h"
#include "utils.h"
#include "stratutils.h"
#include "Bank.h"
#include "Spell.h"
#include "EventSystem.h"
#include "Entity.h"
#include "Combat.h"
#include "ConsoleOutput.h"
#include "CombatStrategy.h"

std::vector<Entity>					s_entities;
std::shared_ptr<ConsoleOutput>		s_console;
Bank<SpellData>						s_spellDataBank;
EventSystem							s_eventSystem;
std::vector<Entity *>				s_party1;
std::vector<Entity *>				s_party2;

static uint new_entity_id();
static void initialize();
static void quit();
static void create_spell_data_bank();
static void create_entities_and_parties();
static void create_console_output();

int main()
{
	initialize();

	Combat combat(&s_eventSystem, &s_spellDataBank, &s_party1, &s_party2);

	while (!combat.Over()) {
		combat.Update();

		s_eventSystem.ProcessEvents();
	}

	quit();

	return 0;
}

static void initialize()
{
	Result result;

	result = stratutils::Initialize(&s_spellDataBank);
	assert(ok(result));

	create_spell_data_bank();

	create_entities_and_parties();

	create_console_output();
}

static void quit()
{
	stratutils::Quit();
}

static void create_spell_data_bank()
{
	Result			result;
	uint			id;
	SpellData		spell;
	std::string		name;

	// Spell: Attack
	name = "Attack";
	spell.effects = {
		SpellEffect::MakeModifyResource(Resource::HP, 3, RESOURCE_MODIF_TYPE_DAMAGE, MagicSchool::Physical)
	};
	spell.targetingData = TargetingData(fTarAlly|fTarOpponent|fTarEntity|fTarDefaultOpponent);
	spell.cooldown = 0;
	result = s_spellDataBank.Insert(spell, name, &id);
	assert(ok(result));

	// Spell: Fire
	name = "Fire";
	spell.effects = {
		SpellEffect::MakeModifyResource(Resource::HP, 4, RESOURCE_MODIF_TYPE_DAMAGE, MagicSchool::Fire)
	};
	spell.targetingData = TargetingData(fTarAlly|fTarOpponent|fTarEntity|fTarParty|fTarDefaultOpponent);
	spell.cooldown = 0;
	result = s_spellDataBank.Insert(spell, name, &id);
	assert(ok(result));

	// Spell: Cure
	name = "Cure";
	spell.effects = {
		SpellEffect::MakeModifyResource(Resource::HP, 6, RESOURCE_MODIF_TYPE_HEAL, MagicSchool::Light)
	};
	spell.targetingData = TargetingData(fTarAlly | fTarOpponent | fTarEntity | fTarParty | fTarDefaultAlly);
	spell.cooldown = 0;
	result = s_spellDataBank.Insert(spell, name, &id);
	assert(ok(result));

	// Spell: Life
	name = "Life";
	spell.effects = {
		SpellEffect::MakeRevive(10, MagicSchool::Light)
	};
	spell.targetingData = TargetingData(fTarAlly | fTarOpponent | fTarEntity | fTarDefaultAlly);
	spell.cooldown = 5;
	result = s_spellDataBank.Insert(spell, name, &id);
	assert(ok(result));

	// Add other spells here...
}

static uint new_entity_id()
{
	static uint id = 0;
	return id++;
}

static void create_entities_and_parties()
{
	Result result;

	// We'll use the same simple ai for all entities for now.
	AIData ai{ { STRATEGYNAME_OFFENSIVE }, 1 };

	uint idFire;
	result = s_spellDataBank.IdFromName(IN "Fire", OUT &idFire);
	assert(ok(result));

	uint idAttack;
	result = s_spellDataBank.IdFromName(IN "Attack", OUT &idAttack);
	assert(ok(result));

	uint idCure;
	result = s_spellDataBank.IdFromName(IN "Cure", OUT &idCure);
	assert(ok(result));

	uint idLife;
	result = s_spellDataBank.IdFromName(IN "Life", OUT &idLife);
	assert(ok(result));

	std::vector<SpellInstance> spells = {
		SpellInstance(idAttack),
		SpellInstance(idFire),
		SpellInstance(idCure)
	};

	s_entities.push_back(Entity("Warrior", new_entity_id(), 80, ai, spells));
	s_entities.push_back(Entity("Mage", new_entity_id(), 60, ai, spells));
	s_entities.push_back(Entity("Skeleton", new_entity_id(), 5, ai, spells));
	s_entities.push_back(Entity("Skeleton", new_entity_id(), 5, ai, spells));

	{
		AIData ai{ { STRATEGYNAME_REVIVE, STRATEGYNAME_OFFENSIVE }, 1 };
		std::vector<SpellInstance> spells = {
			SpellInstance(idFire),
			SpellInstance(idLife)
		};
		s_entities.push_back(Entity("Sorceress", new_entity_id(), 20, ai, spells));
	}

	s_party1.push_back(&s_entities[0]);
	s_party1.push_back(&s_entities[1]);

	s_party2.push_back(&s_entities[2]);
	s_party2.push_back(&s_entities[3]);
	s_party2.push_back(&s_entities[4]);

	// Test
	//kill_entity(s_party2[0]);
}

static void create_console_output()
{
	const std::vector<const Party *> parties{ {&s_party1, &s_party2} };
	s_console = std::make_shared<ConsoleOutput>(&s_eventSystem, &s_spellDataBank, parties);
}