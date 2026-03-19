#include "Form.h"

namespace CppWinForms
{

Form::Form()
{
	InitControl(L"System.Windows.Forms.Form");
}

void Form::AddChild(Control& child)
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

void Form::CenterChild(Control& child)
{
	_variant_t clientSize = type_->InvokeMember_3(
		_bstr_t(L"ClientSize"), __Internal::kGetProperty,
		nullptr, variant_, nullptr);

	long cw = 0, ch = 0;

	if (clientSize.vt == VT_RECORD &&
		clientSize.pRecInfo != nullptr &&
		clientSize.pvRecord != nullptr)
	{
		VARIANT wVar, hVar;
		VariantInit(&wVar);
		VariantInit(&hVar);
		HRESULT hr1 = clientSize.pRecInfo->GetField(
			clientSize.pvRecord, L"Width", &wVar);
		if (FAILED(hr1))
			hr1 = clientSize.pRecInfo->GetField(
				clientSize.pvRecord, L"width", &wVar);
		HRESULT hr2 = clientSize.pRecInfo->GetField(
			clientSize.pvRecord, L"Height", &hVar);
		if (FAILED(hr2))
			hr2 = clientSize.pRecInfo->GetField(
				clientSize.pvRecord, L"height", &hVar);
		if (SUCCEEDED(hr1) && SUCCEEDED(hr2))
		{
			cw = wVar.lVal;
			ch = hVar.lVal;
		}
		VariantClear(&wVar);
		VariantClear(&hVar);
	}
	else
	{
		mscorlib::_TypePtr sizeType =
			__Internal::GetTypeFromVariant(clientSize);
		if (sizeType != nullptr)
		{
			_variant_t cwv = sizeType->InvokeMember_3(
				_bstr_t(L"Width"), __Internal::kGetProperty,
				nullptr, clientSize, nullptr);
			_variant_t chv = sizeType->InvokeMember_3(
				_bstr_t(L"Height"), __Internal::kGetProperty,
				nullptr, clientSize, nullptr);
			cw = static_cast<long>(cwv);
			ch = static_cast<long>(chv);
		}
	}

	if (cw > 0 && ch > 0)
	{
		child.Left = (cw - child.Width) / 2;
		child.Top  = (ch - child.Height) / 2;
	}
	child.Anchor = 0;
}

} // namespace CppWinForms
