#define NOMINMAX
#include "Table.h"

#include <algorithm>
#include <iostream>
#include <cassert>

#include "wincons.h"

Table::Table(
	const TableLayoutInfo &layout,
	const std::vector<TableRow> &rows
)
	:
	m_layout(layout),
	m_rows(rows)
{
	size_t nCols = 0;
	for (auto &row : m_rows) {
		nCols = std::max(nCols, row.entries.size());
	}

	if (0 == nCols)	return;

	m_columnInfos.reserve(nCols);

	// Determine the width of each column
	for (size_t col = 0; col < nCols; col++) {
		int colWidth = 0;
		
		for (size_t row = 0; row < m_rows.size(); row++) {
			if (col >= m_rows[row].NumColumns()) {
				continue;
			}

			const auto &entry = m_rows[row].entries[col];

			int entryWidth = m_layout.colLeftPad + entry.text.size() + m_layout.colRightPad;

			colWidth = std::max(colWidth, entryWidth);
		}

		m_columnInfos.push_back(TableColumnInfo{ colWidth });
	}

	// Determine the frame width
	m_frameWidth = 0;
	for (const auto &info : m_columnInfos) {
		m_frameWidth += info.width + 1;// + 1 for the vertical separation |
	}
	++m_frameWidth;// + 1 for the vertical separation |
}

void Table::Print() const
{
	if (empty()) {
		return;
	}

	// Printing
	std::cout << std::string(m_frameWidth, '-') << std::endl;
	for (size_t i = 0; i< m_rows.size(); i++) {
		print_row(i);
	}
}

void Table::print_row(size_t i) const
{
	assert(i < m_rows.size());

	const auto &row = m_rows[i];

	const std::string kLeftPadStr(m_layout.colLeftPad, ' ');
	const std::string kRightPadStr(m_layout.colRightPad, ' ');

	std::cout << "|";
	for (size_t c = 0; c < m_columnInfos.size(); c++) {
		if (c < row.NumColumns()) {
			std::cout << kLeftPadStr;

			// Center the entry text
			int nBlanks = m_columnInfos[c].width - (m_layout.colLeftPad + m_layout.colRightPad + row.entries[c].text.size());
			assert(nBlanks >= 0);
			
			// Left whitespaces
			std::cout << std::string(nBlanks / 2, ' ');

			// Entry text
			const auto &e = row.entries[c];
			if (e.ignoreTextColor) {
				std::cout << e.text;
			}
			else {
				WinCons::color_printf(e.textColor, WinCons::INTENSIFY, "%s", e.text.c_str());
			}

			// Right whitespaces
			std::cout << std::string(nBlanks - nBlanks / 2, ' ');

			std::cout << kRightPadStr;
		}
		else {
			// Print an empty entry
			std::cout << std::string(m_columnInfos[c].width, ' ');
		}

		std::cout << "|";
	}

	std::cout << std::endl;

	if (row.printHorizBarBelow) {
		std::cout << std::string(m_frameWidth, '-') << std::endl;
	}
}