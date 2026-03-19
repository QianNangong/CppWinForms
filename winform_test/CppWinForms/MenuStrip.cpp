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

	void AddToCollection(
		const _variant_t& owner,
		const wchar_t* collectionProp,
		const _variant_t& item)
	{
		mscorlib::_TypePtr ownerType = __Internal::GetTypeFromVariant(owner);
		_variant_t collection = ownerType->InvokeMember_3(
			_bstr_t(collectionProp), __Internal::kGetProperty,
			nullptr, owner, nullptr);
		mscorlib::_TypePtr collType = __Internal::GetTypeFromVariant(collection);

		_variant_t itemCopy(item);
		SAFEARRAY* args = __Internal::MakeArgArray({ &itemCopy });
		collType->InvokeMember_3(
			_bstr_t(L"Add"), __Internal::kInvokeMethod,
			nullptr, collection, args);
		SafeArrayDestroy(args);
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
		AddToCollection(topItem, L"DropDownItems", subItem);
	}

	AddToCollection(variant_, L"Items", topItem);
}

} // namespace CppWinForms
