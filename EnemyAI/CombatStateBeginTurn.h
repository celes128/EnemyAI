#pragma once

#include "Combat.h"

class CombatStateBeginTurn : public ICombatState {
public:
	CombatStateBeginTurn(Combat *combat);

	void OnEnter() override;
	void Update() override;
};