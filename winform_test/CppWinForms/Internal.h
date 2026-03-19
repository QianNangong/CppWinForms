#pragma once

#include "../ClrInterop.h"
#include <functional>

namespace CppWinForms::__Internal
{

mscorlib::_AppDomainPtr& AppDomain();
mscorlib::_AssemblyPtr&  FormsAssembly();

HRESULT GetDispId(IDispatch* dispatch, const wchar_t* name, DISPID* dispId);
HRESULT GetProperty(IDispatch* dispatch, const wchar_t* name, VARIANT* result);
HRESULT PutProperty(IDispatch* dispatch, const wchar_t* name, const VARIANT& value);

mscorlib::_TypePtr GetTypeFromVariant(const _variant_t& obj);
SAFEARRAY* MakeArgArray(std::initializer_list<const _variant_t*> args);

inline constexpr auto kGetProperty = static_cast<mscorlib::BindingFlags>(
	mscorlib::BindingFlags_Instance |
	mscorlib::BindingFlags_Public |
	mscorlib::BindingFlags_GetProperty);
inline constexpr auto kSetProperty = static_cast<mscorlib::BindingFlags>(
	mscorlib::BindingFlags_Instance |
	mscorlib::BindingFlags_Public |
	mscorlib::BindingFlags_SetProperty);
inline constexpr auto kInvokeMethod = static_cast<mscorlib::BindingFlags>(
	mscorlib::BindingFlags_Instance |
	mscorlib::BindingFlags_Public |
	mscorlib::BindingFlags_InvokeMethod);
inline constexpr auto kStaticInvoke = static_cast<mscorlib::BindingFlags>(
	mscorlib::BindingFlags_Static |
	mscorlib::BindingFlags_Public |
	mscorlib::BindingFlags_InvokeMethod);

void InitEventBridge();
void AttachEvent(
	const _variant_t& control,
	const wchar_t* eventName,
	std::function<void()>* callback);

} // namespace CppWinForms::__Internal
