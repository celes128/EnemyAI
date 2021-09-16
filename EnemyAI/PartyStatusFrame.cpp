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

	// Create the entity rows
	for (auto &party : m_parties) {
		for (size_t i = 0; i < party->size(); i++) {
			const auto &entity = (*party)[i];

			// Determine the row color
			WinCons::Color color;
			if (entity->Alive()) {
				auto percent = static_cast<float>(entity->HP()) / static_cast<float>(entity->MaxHP());
				if (percent > 0.5f) {
					color = WinCons::Color::Green;
				}
				else {
					color = WinCons::Color::Orange;
				}
			}
			else {
				color = WinCons::Color::Red;
			}

			// Create the row entries
			TableRowEntry name(entity->Name(), color);
			TableRowEntry hp(std::to_string(entity->HP()) + "/" + std::to_string(entity->MaxHP()), color);

			// Do not draw an horizontal bar between party members
			bool printHorizBar = (i + 1 != party->size()) ? false : true;

			// Done!
			rows.push_back(TableRow{{{name, hp}}, printHorizBar});
		}
	}
	
	m_table = Table(TableLayoutInfo{ 7, 7 }, rows);
}