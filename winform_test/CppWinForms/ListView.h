#pragma once

#include "Control.h"

namespace CppWinForms
{

class ListView : public Control
{
public:
	// View: LargeIcon=0, Details=1, SmallIcon=2, List=3, Tile=4
	__declspec(property(get = get_View, put = put_View))
	long View;
	__declspec(property(get = get_FullRowSelect, put = put_FullRowSelect))
	bool FullRowSelect;
	__declspec(property(get = get_GridLines, put = put_GridLines))
	bool GridLines;

	long get_View() const         { return GetLongProp(L"View"); }
	void put_View(long v)         { PutLongProp(L"View", v); }
	bool get_FullRowSelect() const;
	void put_FullRowSelect(bool v);
	bool get_GridLines() const;
	void put_GridLines(bool v);

	ListView();

	void AddColumn(const wchar_t* text, long width);
	void AddRow(std::initializer_list<const wchar_t*> cells);
};

} // namespace CppWinForms
