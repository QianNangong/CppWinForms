#include "ClrHost.h"
#include "CppWinForms.h"

int main()
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	if (FAILED(hr))
	{
		PrintHResult("CoInitializeEx failed", hr);
		return -1;
	}

	Microsoft::WRL::ComPtr<ICorRuntimeHost> runtimeHost;
	mscorlib::_AppDomainPtr appDomain;

	try
	{
		hr = StartClr(runtimeHost, appDomain);
		if (FAILED(hr))
		{
			PrintHResult("StartClr failed", hr);
			CoUninitialize();
			return -1;
		}

		CppWinForms::Initialize(appDomain);
		CppWinForms::Application::EnableVisualStyles();

		CppWinForms::Form form;
		form.Text          = L"Native C++ WinForms";
		form.Width         = 640;
		form.Height        = 360;
		form.StartPosition = 1;  // FormStartPosition.CenterScreen

		CppWinForms::Button button;
		button.Text   = L"\u70B9\u51FB\u5F39\u7A97";
		button.Width  = 120;
		button.Height = 40;

		form.AddChild(button);
		form.CenterChild(button);

		button.OnClick([]() {
			MessageBoxW(nullptr,
				L"\u6309\u94AE\u5DF2\u70B9\u51FB\u3002",
				L"\u63D0\u793A",
				MB_OK | MB_ICONINFORMATION);
		});

		CppWinForms::Application::Run(form);
	}
	catch (const _com_error& error)
	{
		PrintComError(error);
		CoUninitialize();
		return -1;
	}

	CoUninitialize();
	return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main();
}
