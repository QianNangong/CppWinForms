#pragma once

#include "Control.h"

namespace CppWinForms
{

class Form : public Control
{
public:
	__declspec(property(get = get_StartPosition, put = put_StartPosition))
	long StartPosition;

	Form();

	void CenterChild(Control& child);
	void SetMainMenuStrip(Control& menuStrip);

	long get_StartPosition() const { return GetLongProp(L"StartPosition"); }
	void put_StartPosition(long v) { PutLongProp(L"StartPosition", v); }
};

} // namespace CppWinForms
