#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <comdef.h>
#include <cstdio>
#include <initializer_list>
#include <mscoree.h>
#include <wrl/client.h>
#include <metahost.h>

#pragma comment(lib, "mscoree.lib")

#import "mscorlib.tlb" \
	rename("ReportEvent", "InteropServices_ReportEvent") \
	rename("or", "InteropServices_or")

#if defined(_WIN64)
inline constexpr wchar_t kFormsAssemblyPath[] =
	L"C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319\\System.Windows.Forms.dll";
inline constexpr wchar_t kSystemDllPath[] =
	L"C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319\\System.dll";
inline constexpr wchar_t kDataDllPath[] =
	L"C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319\\System.Data.dll";
#else
inline constexpr wchar_t kFormsAssemblyPath[] =
	L"C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\System.Windows.Forms.dll";
inline constexpr wchar_t kSystemDllPath[] =
	L"C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\System.dll";
inline constexpr wchar_t kDataDllPath[] =
	L"C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\System.Data.dll";
#endif
