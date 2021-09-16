#include "PartyStatusFrame.h"

#include "Entity.h"

PartyStatusFrame::PartyStatusFrame(const std::vector<const Party *> &parties)
	:
	m_parties(parties)
{}

void PartyStatusFrame::Update()
{
	create_table();
}

void PartyStatusFrame::Print()
{
	m_table.Print();
}

void PartyStatusFrame::create_table()
{
	std::vector<TableRow> rows;

	// Header row
	rows.push_back(TableRow{
		{ {TableRowEntry("Name"), TableRowEntry("HP") }}
		});

	// One row per entity
	for (auto &party : m_parties) {
		for (auto &entity : *party) {
			std::string hpStr = std::to_string(entity->HP()) + "/" + std::to_string(entity->MaxHP());

			rows.push_back(
				TableRow{
					{ {TableRowEntry(entity->Name()), TableRowEntry(hpStr) }}
				}
			);
		}
	}
	
	m_table = Table(TableLayoutInfo{ 7, 7 }, rows);
}