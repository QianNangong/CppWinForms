#pragma once

#include "Form.h"

namespace CppWinForms
{

class Application
{
public:
	static void EnableVisualStyles();
	static void Run(Form& mainForm);
};

} // namespace CppWinForms
