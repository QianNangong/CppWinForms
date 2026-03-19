#pragma once

#include "Control.h"

namespace CppWinForms
{

class MenuStrip : public Control
{
public:
	MenuStrip();

	void AddMenu(const wchar_t* menuText,
		std::initializer_list<const wchar_t*> itemTexts);
};

} // namespace CppWinForms
