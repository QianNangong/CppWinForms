#pragma once

#include "Control.h"

namespace CppWinForms
{

class ToolStrip : public Control
{
public:
	ToolStrip();

	void AddButton(const wchar_t* text,
		std::function<void()> onClick = {});
	void AddSeparator();

private:
	std::vector<std::unique_ptr<std::function<void()>>> itemCallbacks_;
};

} // namespace CppWinForms
