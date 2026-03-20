#include "ProgressBar.h"

namespace CppWinForms
{

ProgressBar::ProgressBar()
{
	InitControl(L"System.Windows.Forms.ProgressBar");
}

void ProgressBar::PerformStep()
{
	type_->InvokeMember_3(
		_bstr_t(L"PerformStep"), __Internal::kInvokeMethod,
		nullptr, variant_, nullptr);
}

void ProgressBar::Increment(long value)
{
	_variant_t val(value);
	SAFEARRAY* a = __Internal::MakeArgArray({ &val });
	type_->InvokeMember_3(
		_bstr_t(L"Increment"), __Internal::kInvokeMethod,
		nullptr, variant_, a);
	SafeArrayDestroy(a);
}

} // namespace CppWinForms
