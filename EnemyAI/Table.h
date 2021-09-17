#pragma once

#include <string>
#include <vector>

#include "wincons.h"

struct TableRowEntry{
	enum TEXTCOLOR_IGNORE_FLAG {
		TEXTCOLOR_IGNORE_FLAG_FALSE = 0,
		TEXTCOLOR_IGNORE_FLAG_TRUE = 1
	};

	TableRowEntry(const std::string &Text)
		: text(Text)
		, ignoreTextColor(TEXTCOLOR_IGNORE_FLAG_TRUE)
	{}

	TableRowEntry(const std::string &Text, WinCons::Color TextColor)
		: text(Text)
		, ignoreTextColor(TEXTCOLOR_IGNORE_FLAG_FALSE)
		, textColor(TextColor)
	{}


	std::string			text;

	bool				ignoreTextColor = true;
	WinCons::Color		textColor;
};

struct TableLayoutInfo {
	int			colLeftPad;		// left padding in columns
	int			colRightPad;	// right padding in columns
};

struct TableColumnInfo {
	int		width;
};

struct TableRow {
	std::vector<TableRowEntry>		entries;
	bool							printHorizBarBelow = true;

	size_t NumColumns() const { return entries.size(); }
	bool ValidColumnIndex(size_t c) const { return c < NumColumns(); }
	bool InvalidColumnIndex(size_t c) const { return !ValidColumnIndex(c); }
};

class Table {
public:
	Table(
		const TableLayoutInfo &layout = {3, 3},
		const std::vector<TableRow> &rows = {}
	);

	void Print() const;

private:
	/*
		PRECONDITIONS
		* r < num_rows()
	*/
	void print_row(size_t r) const;

private:
	// CATEGORY: constructor utils
	void compute_column_infos(size_t nCols);
	void compute_frame_width();
	void cache_graphics_strings();

	// CATEGORY: general utils
	bool empty() const { return m_columnInfos.size() == 0; }
	size_t num_columns() const { return m_columnInfos.size(); }
	size_t num_rows() const { return m_rows.size(); }

	// CATEGORY: print utils
	void print_horiz_bar() const;
	void optional_print_horiz_bar(size_t r) const;
	void print_entry_vert_bar() const;
	void print_entry(size_t c, size_t r) const;

private:
	TableLayoutInfo					m_layout;
	std::vector<TableRow>			m_rows;

	std::vector<TableColumnInfo>	m_columnInfos;

	int								m_frameWidth;

	// (Cached) Graphics strings
	std::string						m_horizBar;// separates the rows
	std::string						m_colLeftPadStr;
	std::string						m_colRightPadStr;
};