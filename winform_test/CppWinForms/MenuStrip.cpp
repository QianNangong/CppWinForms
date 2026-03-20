#include "MenuStrip.h"

namespace CppWinForms
{

namespace
{
	_variant_t CreateMenuItem(const wchar_t* text)
	{
		mscorlib::_ObjectHandlePtr handle =
			__Internal::AppDomain()->CreateInstanceFrom(
				_bstr_t(kFormsAssemblyPath),
				_bstr_t(L"System.Windows.Forms.ToolStripMenuItem"));
		_variant_t item = handle->Unwrap();

		mscorlib::_TypePtr itemType = __Internal::GetTypeFromVariant(item);
		_variant_t textVal(text);
		SAFEARRAY* args = __Internal::MakeArgArray({ &textVal });
		itemType->InvokeMember_3(
			_bstr_t(L"Text"), __Internal::kSetProperty,
			nullptr, item, args);
		SafeArrayDestroy(args);
		return item;
	}
}

MenuStrip::MenuStrip()
{
	InitControl(L"System.Windows.Forms.MenuStrip");
}

void MenuStrip::AddMenu(
	const wchar_t* menuText,
	std::initializer_list<const wchar_t*> itemTexts)
{
	_variant_t topItem = CreateMenuItem(menuText);

	for (const wchar_t* text : itemTexts)
	{
		_variant_t subItem = CreateMenuItem(text);
		__Internal::AddToCollection(topItem, L"DropDownItems", subItem);
	}

	__Internal::AddToCollection(variant_, L"Items", topItem);
}

} // namespace CppWinForms
