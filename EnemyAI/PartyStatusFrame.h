#pragma once

#include <vector>

#include "Party.h"
#include "Table.h"

class PartyStatusFrame {
public:
	PartyStatusFrame(const std::vector<const Party *> &parties);

	void Update();
	void Print();

private:
	void create_table();

private:
	const std::vector<const Party *>	m_parties;
	Table								m_table;
};