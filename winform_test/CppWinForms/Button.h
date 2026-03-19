#pragma once

#include "Control.h"

namespace CppWinForms
{

class Button : public Control
{
public:
	Button();

	void OnClick(std::function<void()> callback)
	{
		AddEventHandler(L"Click", std::move(callback));
	}
};

} // namespace CppWinForms
