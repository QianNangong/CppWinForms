#pragma once

#include "ClrInterop.h"

void PrintHResult(const char* message, HRESULT hr);
void PrintComError(const _com_error& error);

HRESULT StartClr(
	Microsoft::WRL::ComPtr<ICorRuntimeHost>& runtimeHost,
	mscorlib::_AppDomainPtr& appDomain);
