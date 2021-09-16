#pragma once

#include "Combat.h"
#include "Action.h"

class CombatStateTurn : public ICombatState {
public:
	CombatStateTurn(Combat *combat);

	void OnEnter() override;
	void Update() override;

private:
	Action choose_action(const ReadyEntity &re);
	void process_action(Action action);
	
	/*
		PRECONDITIONS
		* action.type == Action::Type::SpellCast
	*/
	void process_action_spellcast(Action action);

	/*
		PRECONDITIONS
		* action.type == Action::Type::None
	*/
	void process_action_none(Action action);

	/*
		PRECONDITIONS
		* caster != nullptr
	*/
	void process_spell(const SpellData &spell, Entity *caster, std::vector<Entity *> &targets);

	void process_spell_effect(const SpellEffect &effect, Entity *caster, Entity *target);

	/*
		PRECONDITIONS
		* effect.type == SpellEffect::Type::ModifyResource
	*/
	void process_spell_effect_resource(const SpellEffect &effect, Entity *caster, Entity *target);
};