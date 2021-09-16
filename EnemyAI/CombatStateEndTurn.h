#pragma once

#include "Combat.h"

class CombatStateEndTurn : public ICombatState {
public:
	CombatStateEndTurn(Combat *combat);

	void OnEnter() override;
	void Update() override;
};