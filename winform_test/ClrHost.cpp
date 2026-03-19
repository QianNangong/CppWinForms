#include "ClrHost.h"

using Microsoft::WRL::ComPtr;

void PrintHResult(const char* message, HRESULT hr)
{
	std::printf("%s: 0x%08X\n", message, static_cast<unsigned int>(hr));
}

void PrintComError(const _com_error& error)
{
	PrintHResult("COM exception", error.Error());

	const wchar_t* source = error.Source();
	if (source != nullptr && *source != L'\0')
	{
		::wprintf(L"Source: %ls\n", source);
	}

	const wchar_t* description = error.Description();
	if (description != nullptr && *description != L'\0')
	{
		::wprintf(L"Description: %ls\n", description);
	}
}

HRESULT StartClr(
	ComPtr<ICorRuntimeHost>& runtimeHost,
	mscorlib::_AppDomainPtr& appDomain)
{
	runtimeHost.Reset();
	appDomain = nullptr;

	ComPtr<ICLRMetaHost> metaHost;
	ComPtr<ICLRRuntimeInfo> runtimeInfo;
	ComPtr<ICorRuntimeHost> host;

	HRESULT hr = CLRCreateInstance(
		CLSID_CLRMetaHost, IID_ICLRMetaHost,
		reinterpret_cast<void**>(metaHost.GetAddressOf()));
	if (FAILED(hr)) return hr;

	hr = metaHost->GetRuntime(
		L"v4.0.30319", IID_ICLRRuntimeInfo,
		reinterpret_cast<void**>(runtimeInfo.GetAddressOf()));
	if (FAILED(hr)) return hr;

	BOOL loadable = FALSE;
	hr = runtimeInfo->IsLoadable(&loadable);
	if (FAILED(hr) || loadable == FALSE)
		return FAILED(hr) ? hr : E_FAIL;

	hr = runtimeInfo->GetInterface(
		CLSID_CorRuntimeHost, IID_ICorRuntimeHost,
		reinterpret_cast<void**>(host.GetAddressOf()));
	if (FAILED(hr)) return hr;

	hr = host->Start();
	if (FAILED(hr)) return hr;

	ComPtr<IUnknown> defaultDomain;
	hr = host->GetDefaultDomain(defaultDomain.GetAddressOf());
	if (FAILED(hr)) return hr;

	mscorlib::_AppDomain* rawDomain = nullptr;
	hr = defaultDomain->QueryInterface(
		__uuidof(mscorlib::_AppDomain),
		reinterpret_cast<void**>(&rawDomain));
	if (FAILED(hr)) return hr;

	appDomain.Attach(rawDomain);
	runtimeHost = host;
	return S_OK;
}
