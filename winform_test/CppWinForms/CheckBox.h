#pragma once

#include "Control.h"

namespace CppWinForms
{

class CheckBox : public Control
{
public:
	__declspec(property(get = get_Checked, put = put_Checked))
	bool Checked;
	__declspec(property(get = get_AutoSize, put = put_AutoSize))
	bool AutoSize;

	bool get_Checked() const  { return GetBoolProp(L"Checked"); }
	void put_Checked(bool v)  { PutBoolProp(L"Checked", v); }
	bool get_AutoSize() const { return GetBoolProp(L"AutoSize"); }
	void put_AutoSize(bool v) { PutBoolProp(L"AutoSize", v); }

	CheckBox();

	void OnCheckedChanged(std::function<void()> callback)
	{
		AddEventHandler(L"CheckedChanged", std::move(callback));
	}

	void OnClick(std::function<void()> callback)
	{
		AddEventHandler(L"Click", std::move(callback));
	}
};

} // namespace CppWinForms
