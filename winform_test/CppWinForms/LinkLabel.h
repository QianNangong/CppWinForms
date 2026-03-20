#pragma once

#include "Control.h"

namespace CppWinForms
{

class LinkLabel : public Control
{
public:
	__declspec(property(get = get_AutoSize, put = put_AutoSize))
	bool AutoSize;

	bool get_AutoSize() const { return GetBoolProp(L"AutoSize"); }
	void put_AutoSize(bool v) { PutBoolProp(L"AutoSize", v); }

	LinkLabel();

	void OnLinkClicked(std::function<void()> callback)
	{
		AddEventHandler(L"LinkClicked", std::move(callback));
	}
};

} // namespace CppWinForms
