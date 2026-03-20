#include "ListBox.h"

namespace CppWinForms
{

ListBox::ListBox()
{
	InitControl(L"System.Windows.Forms.ListBox");
}

void ListBox::AddItem(const wchar_t* text)
{
	_variant_t items = type_->InvokeMember_3(
		_bstr_t(L"Items"), __Internal::kGetProperty,
		nullptr, variant_, nullptr);
	mscorlib::_TypePtr itemsType = __Internal::GetTypeFromVariant(items);

	_variant_t textVar(text);
	SAFEARRAY* a = __Internal::MakeArgArray({ &textVar });
	itemsType->InvokeMember_3(
		_bstr_t(L"Add"), __Internal::kInvokeMethod,
		nullptr, items, a);
	SafeArrayDestroy(a);
}

void ListBox::ClearItems()
{
	_variant_t items = type_->InvokeMember_3(
		_bstr_t(L"Items"), __Internal::kGetProperty,
		nullptr, variant_, nullptr);
	mscorlib::_TypePtr itemsType = __Internal::GetTypeFromVariant(items);
	itemsType->InvokeMember_3(
		_bstr_t(L"Clear"), __Internal::kInvokeMethod,
		nullptr, items, nullptr);
}

} // namespace CppWinForms
