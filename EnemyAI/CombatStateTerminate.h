#pragma once

#include "Combat.h"

class CombatStateTerminate : public ICombatState {
public:
	CombatStateTerminate(Combat *combat);

	void OnEnter() override;
	void Update() override;
};