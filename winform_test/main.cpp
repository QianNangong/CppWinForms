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
		form.Width         = 800;
		form.Height        = 480;
		form.StartPosition = 1;  // CenterScreen

		// --- MenuStrip ---
		CppWinForms::MenuStrip menu;
		menu.AddMenu(L"\u6587\u4EF6(&F)",  // 文件
			{ L"\u65B0\u5EFA(&N)", L"\u6253\u5F00(&O)", L"\u4FDD\u5B58(&S)", L"\u9000\u51FA(&X)" });
		menu.AddMenu(L"\u7F16\u8F91(&E)",  // 编辑
			{ L"\u526A\u5207(&T)", L"\u590D\u5236(&C)", L"\u7C98\u8D34(&V)" });
		menu.AddMenu(L"\u5E2E\u52A9(&H)",  // 帮助
			{ L"\u5173\u4E8E(&A)" });

		// --- ListView (Report / Details mode) ---
		CppWinForms::ListView listView;
		listView.View          = 1;     // Details
		listView.FullRowSelect = true;
		listView.GridLines     = true;
		listView.Dock          = 5;     // Fill

		listView.AddColumn(L"ID",     50);
		listView.AddColumn(L"\u59D3\u540D", 120);   // 姓名
		listView.AddColumn(L"\u72B6\u6001", 100);   // 状态
		listView.AddColumn(L"\u65E5\u671F", 140);   // 日期
		listView.AddColumn(L"\u5907\u6CE8", 200);   // 备注

		listView.AddRow({ L"1", L"Alice",   L"\u6D3B\u8DC3", L"2025-01-15", L"\u7BA1\u7406\u5458" });
		listView.AddRow({ L"2", L"Bob",     L"\u505C\u7528", L"2025-02-20", L"\u5DF2\u79BB\u804C" });
		listView.AddRow({ L"3", L"Charlie", L"\u6D3B\u8DC3", L"2025-03-10", L"\u5F00\u53D1\u7EC4" });
		listView.AddRow({ L"4", L"Diana",   L"\u5F85\u5BA1", L"2025-04-05", L"\u65B0\u5458\u5DE5" });
		listView.AddRow({ L"5", L"Eve",     L"\u6D3B\u8DC3", L"2025-05-18", L"\u8FD0\u7EF4\u7EC4" });

		// --- Button (docked at bottom) ---
		CppWinForms::Button button;
		button.Text   = L"\u70B9\u51FB\u5F39\u7A97";
		button.Height = 36;
		button.Dock   = 2;  // Bottom

		button.OnClick([]() {
			MessageBoxW(nullptr,
				L"\u6309\u94AE\u5DF2\u70B9\u51FB\u3002",
				L"\u63D0\u793A",
				MB_OK | MB_ICONINFORMATION);
		});

		// Add controls — order matters for dock layout:
		// last-added is front of Z-order, docked first
		form.AddChild(listView);   // Fill  (back)
		form.AddChild(button);     // Bottom
		form.AddChild(menu);       // Top   (front)
		form.SetMainMenuStrip(menu);

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
