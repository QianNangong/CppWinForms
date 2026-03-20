#include "SplitContainer.h"

namespace CppWinForms
{

SplitContainer::SplitContainer()
{
	InitControl(L"System.Windows.Forms.SplitContainer");
}

void SplitContainer::EnsurePanels()
{
	if (panelsInit_) return;
	_variant_t p1 = type_->InvokeMember_3(
		_bstr_t(L"Panel1"), __Internal::kGetProperty,
		nullptr, variant_, nullptr);
	panel1_.Attach(p1);
	_variant_t p2 = type_->InvokeMember_3(
		_bstr_t(L"Panel2"), __Internal::kGetProperty,
		nullptr, variant_, nullptr);
	panel2_.Attach(p2);
	panelsInit_ = true;
}

Control& SplitContainer::Panel1()
{
	EnsurePanels();
	return panel1_;
}

Control& SplitContainer::Panel2()
{
	EnsurePanels();
	return panel2_;
}

} // namespace CppWinForms
