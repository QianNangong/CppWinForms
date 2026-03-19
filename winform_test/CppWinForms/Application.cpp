#include "Application.h"

namespace CppWinForms
{

void Application::EnableVisualStyles()
{
	mscorlib::_TypePtr appType =
		__Internal::FormsAssembly()->GetType_2(
			_bstr_t(L"System.Windows.Forms.Application"));
	appType->InvokeMember_3(
		_bstr_t(L"EnableVisualStyles"), __Internal::kStaticInvoke,
		nullptr, _variant_t(), nullptr);
}

void Application::Run(Form& mainForm)
{
	mscorlib::_TypePtr appType =
		__Internal::FormsAssembly()->GetType_2(
			_bstr_t(L"System.Windows.Forms.Application"));
	_variant_t formVar = mainForm.variant();
	SAFEARRAY* args = __Internal::MakeArgArray({ &formVar });
	appType->InvokeMember_3(
		_bstr_t(L"Run"), __Internal::kStaticInvoke,
		nullptr, _variant_t(), args);
	SafeArrayDestroy(args);
}

} // namespace CppWinForms
