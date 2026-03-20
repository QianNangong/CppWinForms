#include "Control.h"

namespace CppWinForms
{

void Initialize(mscorlib::_AppDomainPtr& appDomain)
{
	__Internal::AppDomain() = appDomain;

	mscorlib::_ObjectHandlePtr handle = appDomain->CreateInstanceFrom(
		_bstr_t(kFormsAssemblyPath),
		_bstr_t(L"System.Windows.Forms.Control"));
	_variant_t obj = handle->Unwrap();
	__Internal::FormsAssembly() =
		__Internal::GetTypeFromVariant(obj)->GetAssembly();

	__Internal::InitEventBridge();
}

void Control::InitControl(const wchar_t* typeName)
{
	mscorlib::_ObjectHandlePtr handle =
		__Internal::AppDomain()->CreateInstanceFrom(
			_bstr_t(kFormsAssemblyPath), _bstr_t(typeName));
	variant_ = handle->Unwrap();
	if (variant_.vt != VT_DISPATCH || variant_.pdispVal == nullptr)
		_com_issue_error(E_FAIL);
	dispatch_ = variant_.pdispVal;
	type_ = __Internal::GetTypeFromVariant(variant_);
}

_bstr_t Control::get_Text() const
{
	_variant_t result;
	__Internal::GetProperty(dispatch_, L"Text", &result);
	return _bstr_t(result);
}

void Control::put_Text(const wchar_t* value)
{
	__Internal::PutProperty(dispatch_, L"Text", _variant_t(value));
}

bool Control::get_Visible() const
{
	return GetBoolProp(L"Visible");
}

void Control::put_Visible(bool v)
{
	PutBoolProp(L"Visible", v);
}

long Control::GetLongProp(const wchar_t* name) const
{
	_variant_t result;
	__Internal::GetProperty(dispatch_, name, &result);
	return static_cast<long>(result);
}

void Control::PutLongProp(const wchar_t* name, long value)
{
	__Internal::PutProperty(dispatch_, name, _variant_t(value));
}

bool Control::GetBoolProp(const wchar_t* name) const
{
	_variant_t result;
	__Internal::GetProperty(dispatch_, name, &result);
	return result.boolVal == VARIANT_TRUE;
}

void Control::PutBoolProp(const wchar_t* name, bool value)
{
	__Internal::PutProperty(dispatch_, name,
		_variant_t(value ? VARIANT_TRUE : VARIANT_FALSE, VT_BOOL));
}

void Control::AddChild(Control& child)
{
	_variant_t controlsObj = type_->InvokeMember_3(
		_bstr_t(L"Controls"), __Internal::kGetProperty,
		nullptr, variant_, nullptr);
	mscorlib::_TypePtr controlsType =
		__Internal::GetTypeFromVariant(controlsObj);

	_variant_t childVar = child.variant();
	SAFEARRAY* args = __Internal::MakeArgArray({ &childVar });
	controlsType->InvokeMember_3(
		_bstr_t(L"Add"), __Internal::kInvokeMethod,
		nullptr, controlsObj, args);
	SafeArrayDestroy(args);
}

void Control::Attach(const _variant_t& managedObject)
{
	variant_ = managedObject;
	if (variant_.vt == VT_DISPATCH && variant_.pdispVal != nullptr)
		dispatch_ = variant_.pdispVal;
	type_ = __Internal::GetTypeFromVariant(variant_);
}

void Control::AddEventHandler(const wchar_t* eventName, std::function<void()> callback)
{
	auto ptr = std::make_unique<std::function<void()>>(std::move(callback));
	auto* raw = ptr.get();
	callbacks_.push_back(std::move(ptr));
	__Internal::AttachEvent(variant_, eventName, raw);
}

} // namespace CppWinForms
