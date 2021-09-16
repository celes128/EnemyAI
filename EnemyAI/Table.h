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
		* i < m_rows.size()
	*/
	void print_row(size_t i) const;

private:
	bool empty() const { return m_columnInfos.size() == 0; }

private:
	TableLayoutInfo					m_layout;
	std::vector<TableRow>			m_rows;

	std::vector<TableColumnInfo>	m_columnInfos;

	int								m_frameWidth;
};