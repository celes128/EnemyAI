#define NOMINMAX
#include "Table.h"

#include <algorithm>
#include <iostream>
#include <cassert>

#include "wincons.h"

static size_t num_columns_from_rows(const std::vector<TableRow> &rows)
{
	size_t nCols = 0;
	for (auto &row : rows) {
		nCols = std::max(nCols, row.NumColumns());
	}

	return nCols;
}

Table::Table(
	const TableLayoutInfo &layout,
	const std::vector<TableRow> &rows
)
	:
	m_layout(layout),
	m_rows(rows)
{	
	auto nCols = num_columns_from_rows(m_rows);
	
	compute_column_infos(nCols);
	
	compute_frame_width();

	cache_graphics_strings();
}

void Table::Print() const
{
	if (empty()) {
		return;
	}

	// Top horizontal bar of the table frame
	print_horiz_bar();	

	for (size_t r = 0; r < num_rows(); r++) {
		print_row(r);

		// The printing of the other horizontal bars is decided
		// by the row. (flag printHorizBarBelow in the TableRow struct)
		optional_print_horiz_bar(r);
	}
}

void Table::print_row(size_t r) const
{
	assert(r < num_rows());

	print_entry_vert_bar();
	for (size_t c = 0; c < num_columns(); c++) {
		print_entry(c, r);

		print_entry_vert_bar();
	}

	std::cout << std::endl;
}

void Table::compute_column_infos(size_t nCols)
{
	m_columnInfos.reserve(nCols);

	// Determine the width of each column
	for (size_t c = 0; c < nCols; c++) {
		int columnWidth = 0;

		for (size_t r = 0; r < num_rows(); r++) {
			const auto &row = m_rows[r];

			if (row.InvalidColumnIndex(c)) {
				continue;
			}

			const auto &entry = row.entries[c];

			int entryWidth = m_layout.colLeftPad + entry.text.size() + m_layout.colRightPad;

			columnWidth = std::max(columnWidth, entryWidth);
		}

		m_columnInfos.push_back(TableColumnInfo{ columnWidth });
	}
}

void Table::compute_frame_width()
{
	m_frameWidth = 0;
	for (const auto &info : m_columnInfos) {
		m_frameWidth += info.width + 1;// + 1 for the vertical separation |
	}
	++m_frameWidth;// + 1 for the vertical separation |
}

void Table::cache_graphics_strings()
{
	m_horizBar = std::string(m_frameWidth, '-');

	m_colLeftPadStr = std::string(m_layout.colLeftPad, ' ');
	m_colRightPadStr = std::string(m_layout.colRightPad, ' ');
}

void Table::print_horiz_bar() const
{
	std::cout << m_horizBar << std::endl;
}

void Table::optional_print_horiz_bar(size_t r) const
{
	if (m_rows[r].printHorizBarBelow) {
		print_horiz_bar();
	}
}

void Table::print_entry_vert_bar() const
{
	std::cout << "|";
}

void Table::print_entry(size_t c, size_t r) const
{
	const auto &row = m_rows[r];

	if (row.ValidColumnIndex(c)) {
		// Column left padding
		std::cout << m_colLeftPadStr;

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
			WinCons::color_printf(e.textColor, WinCons::DO_NOT_INTENSIFY, "%s", e.text.c_str());
		}

		// Right whitespaces
		std::cout << std::string(nBlanks - nBlanks / 2, ' ');

		// Column right padding
		std::cout << m_colRightPadStr;
	}
	else {
		// Print an empty entry
		std::cout << std::string(m_columnInfos[c].width, ' ');
	}
}