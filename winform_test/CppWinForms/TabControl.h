#pragma once

#include "Control.h"

namespace CppWinForms
{

class TabPage : public Control
{
public:
	TabPage();
};

class TabControl : public Control
{
public:
	__declspec(property(get = get_SelectedIndex, put = put_SelectedIndex))
	long SelectedIndex;

	long get_SelectedIndex() const { return GetLongProp(L"SelectedIndex"); }
	void put_SelectedIndex(long v) { PutLongProp(L"SelectedIndex", v); }

	TabControl();
};

} // namespace CppWinForms
