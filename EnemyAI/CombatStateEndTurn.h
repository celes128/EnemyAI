#pragma once

#include "Combat.h"

class CombatStateEndTurn : public ICombatState {
public:
	CombatStateEndTurn(Combat *combat);

	void OnEnter() override;
	void Update() override;

private:
	void tick_buffs();
	void advance_spell_cooldowns();
};