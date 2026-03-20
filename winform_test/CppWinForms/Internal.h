#pragma once

#include "../ClrInterop.h"
#include <functional>
#include <string>
#include <vector>

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
	const _variant_t& target,
	const wchar_t* eventName,
	std::function<void()>* callback);
void AddToCollection(
	const _variant_t& owner,
	const wchar_t* collectionProp,
	const _variant_t& item);
void AttachVirtualMode(
	const _variant_t& listView,
	int itemCount,
	int columnCount,
	std::function<std::vector<std::wstring>(int)>* provider);
void AttachDgvVirtualMode(
	const _variant_t& dgv,
	int rowCount,
	std::function<std::wstring(int, int)>* provider);

} // namespace CppWinForms::__Internal
