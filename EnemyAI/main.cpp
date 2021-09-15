#include <iostream>
#include <memory>

#include "framework.h"
#include "utils.h"
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

int main()
{
	initialize();

	Combat combat(&s_eventSystem, &s_party1, &s_party2);

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

	s_console = std::make_shared<ConsoleOutput>(&s_eventSystem);

	result = ok(strat::Initialize(&s_spellDataBank));
	assert(result);

	create_spell_data_bank();

	create_entities_and_parties();
}

static void quit()
{
	strat::Quit();
}

static void create_spell_data_bank()
{
	uint id;
	SpellData spell;

	// Spell: Attack
	spell.effects = {
		SpellEffect::MakeModifyResource(Resource::HP, 6, MagicSchool::Physical)
	};
	spell.targetingData = TargetingData(fTarAlly|fTarOpponent|fTarEntity|fTarDefaultOpponent);
	spell.cooldown = 1;
	assert(ok(s_spellDataBank.Insert(spell, "Attack", &id)));

	// Spell: Fire
	spell.effects = {
		SpellEffect::MakeModifyResource(Resource::HP, 6, MagicSchool::Fire)
	};
	spell.targetingData = TargetingData(fTarAlly|fTarOpponent|fTarEntity|fTarParty|fTarDefaultOpponent);
	spell.cooldown = 1;
	assert(ok(s_spellDataBank.Insert(spell, "Fire", &id)));

	// Add other spells here...
}

static uint new_entity_id()
{
	static uint id = 0;
	return id++;
}

static void create_entities_and_parties()
{
	// We'll use the same simple ai for all entities for now.
	AIData ai{ { STRATEGYNAME_OFFENSIVE }, 1 };

	std::vector<SpellInstance> spells = {
		SpellInstance(0, 1),
		SpellInstance(1, 1)
	};

	s_entities.push_back(Entity("Warrior", new_entity_id(), 80, ai, spells));
	s_entities.push_back(Entity("Mage", new_entity_id(), 60, ai, spells));
	s_entities.push_back(Entity("Skeleton", new_entity_id(), 30, ai, spells));
	s_entities.push_back(Entity("Wolf", new_entity_id(), 40, ai, spells));

	s_party1.push_back(&s_entities[0]);
	s_party1.push_back(&s_entities[1]);

	s_party2.push_back(&s_entities[2]);
	s_party2.push_back(&s_entities[3]);

	// Test
	//kill_entity(s_party2[0]);
}