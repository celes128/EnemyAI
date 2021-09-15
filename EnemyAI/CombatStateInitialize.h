#pragma once

#include "Combat.h"

class CombatStateInitialize : public ICombatState {
public:
	CombatStateInitialize(Combat *combat);

	void OnEnter() override;
	void Update() override;
};