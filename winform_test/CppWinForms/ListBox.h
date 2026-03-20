#pragma once

#include "Control.h"

namespace CppWinForms
{

// SelectionMode: None=0, One=1, MultiSimple=2, MultiExtended=3
class ListBox : public Control
{
public:
	__declspec(property(get = get_SelectedIndex, put = put_SelectedIndex))
	long SelectedIndex;
	__declspec(property(get = get_SelectionMode, put = put_SelectionMode))
	long SelectionMode;

	long get_SelectedIndex() const { return GetLongProp(L"SelectedIndex"); }
	void put_SelectedIndex(long v) { PutLongProp(L"SelectedIndex", v); }
	long get_SelectionMode() const { return GetLongProp(L"SelectionMode"); }
	void put_SelectionMode(long v) { PutLongProp(L"SelectionMode", v); }

	ListBox();

	void AddItem(const wchar_t* text);
	void ClearItems();

	void OnSelectedIndexChanged(std::function<void()> callback)
	{
		AddEventHandler(L"SelectedIndexChanged", std::move(callback));
	}
};

} // namespace CppWinForms
