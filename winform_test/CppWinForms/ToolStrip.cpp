#include "ToolStrip.h"

namespace CppWinForms
{

namespace
{
	_variant_t CreateToolStripItem(const wchar_t* typeName)
	{
		mscorlib::_ObjectHandlePtr handle =
			__Internal::AppDomain()->CreateInstanceFrom(
				_bstr_t(kFormsAssemblyPath), _bstr_t(typeName));
		return handle->Unwrap();
	}

	void SetReflectionProp(
		const _variant_t& obj, const wchar_t* name, const _variant_t& value)
	{
		mscorlib::_TypePtr type = __Internal::GetTypeFromVariant(obj);
		_variant_t val(value);
		SAFEARRAY* a = __Internal::MakeArgArray({ &val });
		type->InvokeMember_3(
			_bstr_t(name), __Internal::kSetProperty,
			nullptr, obj, a);
		SafeArrayDestroy(a);
	}
}

ToolStrip::ToolStrip()
{
	InitControl(L"System.Windows.Forms.ToolStrip");
}

void ToolStrip::AddButton(const wchar_t* text, std::function<void()> onClick)
{
	_variant_t btn = CreateToolStripItem(
		L"System.Windows.Forms.ToolStripButton");

	SetReflectionProp(btn, L"Text", _variant_t(text));

	if (onClick)
	{
		auto ptr = std::make_unique<std::function<void()>>(std::move(onClick));
		auto* raw = ptr.get();
		itemCallbacks_.push_back(std::move(ptr));
		__Internal::AttachEvent(btn, L"Click", raw);
	}

	__Internal::AddToCollection(variant_, L"Items", btn);
}

void ToolStrip::AddSeparator()
{
	_variant_t sep = CreateToolStripItem(
		L"System.Windows.Forms.ToolStripSeparator");
	__Internal::AddToCollection(variant_, L"Items", sep);
}

} // namespace CppWinForms
