#include "ClrHost.h"
#include "CppWinForms.h"
#include <string>

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
		menu.AddMenu(L"\u6587\u4EF6(&F)",
			{ L"\u65B0\u5EFA(&N)", L"\u6253\u5F00(&O)",
			  L"\u4FDD\u5B58(&S)", L"\u9000\u51FA(&X)" });
		menu.AddMenu(L"\u7F16\u8F91(&E)",
			{ L"\u526A\u5207(&T)", L"\u590D\u5236(&C)", L"\u7C98\u8D34(&V)" });
		menu.AddMenu(L"\u5E2E\u52A9(&H)", { L"\u5173\u4E8E(&A)" });

		// --- ToolStrip ---
		CppWinForms::ToolStrip toolStrip;
		toolStrip.AddButton(L"\u65B0\u5EFA", []() {        // 新建
			MessageBoxW(nullptr, L"\u65B0\u5EFA", L"ToolStrip", MB_OK);
		});
		toolStrip.AddButton(L"\u6253\u5F00", []() {        // 打开
			MessageBoxW(nullptr, L"\u6253\u5F00", L"ToolStrip", MB_OK);
		});
		toolStrip.AddSeparator();
		toolStrip.AddButton(L"\u4FDD\u5B58");               // 保存

		// --- TabControl with 3 tabs ---
		CppWinForms::TabControl tabs;
		tabs.Dock = 5;  // Fill

		// Tab 1: DataGridView (virtual mode)
		CppWinForms::TabPage tabDgv;
		tabDgv.Text = L"DataGridView";

		CppWinForms::DataGridView dgv;
		dgv.Dock               = 5;
		dgv.ReadOnly           = true;
		dgv.AllowUserToAddRows = false;
		dgv.RowHeadersVisible  = false;
		dgv.SelectionMode      = 1;  // FullRowSelect

		dgv.AddTextColumn(L"ID", 60);
		dgv.AddTextColumn(L"\u59D3\u540D", 120);
		dgv.AddTextColumn(L"\u72B6\u6001", 100);
		dgv.AddTextColumn(L"\u65E5\u671F", 140);
		dgv.AddTextColumn(L"\u5907\u6CE8", 200);

		static constexpr const wchar_t* kStatus[] = {
			L"\u6D3B\u8DC3", L"\u505C\u7528", L"\u5F85\u5BA1"
		};
		static constexpr const wchar_t* kDept[] = {
			L"\u7814\u53D1\u90E8", L"\u8FD0\u7EF4\u7EC4",
			L"\u5E02\u573A\u90E8", L"\u8D22\u52A1\u90E8",
			L"\u4EBA\u4E8B\u90E8"
		};

		dgv.SetVirtualMode(100000, [](int row, int col) -> std::wstring {
			switch (col) {
			case 0: return std::to_wstring(row + 1);
			case 1: return L"User " + std::to_wstring(row + 1);
			case 2: return kStatus[row % 3];
			case 3: return L"2025-" + std::to_wstring(row % 12 + 1)
						+ L"-" + std::to_wstring(row % 28 + 1);
			case 4: return kDept[row % 5];
			default: return L"";
			}
		});
		tabDgv.AddChild(dgv);

		// Tab 2: SplitContainer
		CppWinForms::TabPage tabSplit;
		tabSplit.Text = L"SplitContainer";

		CppWinForms::SplitContainer split;
		split.Dock             = 5;
		split.Orientation      = 1;  // Vertical
		split.SplitterDistance = 300;

		CppWinForms::DataGrid dg;
		dg.Dock           = 5;
		dg.ReadOnly       = true;
		dg.CaptionVisible = false;
		dg.SetSimpleData(
			{ L"ID", L"\u59D3\u540D", L"\u90E8\u95E8" },
			{
				{ L"1", L"Alice",   L"\u7814\u53D1\u90E8" },
				{ L"2", L"Bob",     L"\u8FD0\u7EF4\u7EC4" },
				{ L"3", L"Charlie", L"\u5E02\u573A\u90E8" },
			});
		split.Panel1().AddChild(dg);

		CppWinForms::ListView lv;
		lv.View          = 1;
		lv.FullRowSelect = true;
		lv.GridLines     = true;
		lv.Dock          = 5;
		lv.AddColumn(L"\u5C5E\u6027", 120);
		lv.AddColumn(L"\u503C", 200);
		lv.AddRow({ L"\u6846\u67B6",   L"CppWinForms" });
		lv.AddRow({ L"\u8FD0\u884C\u65F6", L".NET Framework 4" });
		lv.AddRow({ L"\u8BED\u8A00",   L"C++20" });
		split.Panel2().AddChild(lv);

		tabSplit.AddChild(split);

		// Tab 3: DataGrid (legacy)
		CppWinForms::TabPage tabLegacy;
		tabLegacy.Text = L"DataGrid";

		CppWinForms::DataGrid dgLegacy;
		dgLegacy.Dock = 5;
		dgLegacy.SetSimpleData(
			{ L"ID", L"\u59D3\u540D", L"\u72B6\u6001", L"\u65E5\u671F" },
			{
				{ L"1", L"Alice",   L"\u6D3B\u8DC3", L"2025-01-15" },
				{ L"2", L"Bob",     L"\u505C\u7528", L"2025-02-20" },
				{ L"3", L"Charlie", L"\u6D3B\u8DC3", L"2025-03-10" },
				{ L"4", L"Diana",   L"\u5F85\u5BA1", L"2025-04-05" },
				{ L"5", L"Eve",     L"\u6D3B\u8DC3", L"2025-05-18" },
			});
		tabLegacy.AddChild(dgLegacy);

		// Tab 4: Basic controls demo
		CppWinForms::TabPage tabControls;
		tabControls.Text = L"\u57FA\u7840\u63A7\u4EF6";  // 基础控件

		CppWinForms::Label lbl;
		lbl.Text      = L"\u63A7\u4EF6\u6F14\u793A";  // 控件演示
		lbl.AutoSize  = true;
		lbl.Left      = 12;
		lbl.Top       = 12;

		CppWinForms::LinkLabel link;
		link.Text     = L"GitHub \u4ED3\u5E93";  // GitHub 仓库
		link.AutoSize = true;
		link.Left     = 12;
		link.Top      = 36;
		link.OnLinkClicked([]() {
			MessageBoxW(nullptr,
				L"https://github.com/QianNangong/CppWinForms",
				L"Link", MB_OK);
		});

		CppWinForms::ListBox lb;
		lb.Left   = 12;
		lb.Top    = 64;
		lb.Width  = 150;
		lb.Height = 180;
		for (int i = 1; i <= 8; ++i)
			lb.AddItem((L"\u9879\u76EE " + std::to_wstring(i)).c_str());

		CppWinForms::Label lblStatus;
		lblStatus.Text     = L"\u672A\u9009\u62E9";  // 未选择
		lblStatus.AutoSize = true;
		lblStatus.Left     = 12;
		lblStatus.Top      = 252;
		lb.OnSelectedIndexChanged([&]() {
			long idx = lb.SelectedIndex;
			std::wstring msg = L"\u5DF2\u9009: \u9879\u76EE "
				+ std::to_wstring(idx + 1);
			lblStatus.Text = msg.c_str();
		});

		CppWinForms::ProgressBar pb;
		pb.Left    = 180;
		pb.Top     = 64;
		pb.Width   = 300;
		pb.Height  = 28;
		pb.Minimum = 0;
		pb.Maximum = 100;
		pb.Step    = 10;
		pb.Value   = 30;

		CppWinForms::Button btnStep;
		btnStep.Text   = L"\u6B65\u8FDB +10";  // 步进
		btnStep.Left   = 180;
		btnStep.Top    = 100;
		btnStep.Width  = 90;
		btnStep.Height = 28;
		btnStep.OnClick([&]() {
			long v = pb.Value + 10;
			pb.Value = (v <= 100) ? v : 0;
		});

		CppWinForms::PictureBox pic;
		pic.Left        = 180;
		pic.Top         = 140;
		pic.Width       = 200;
		pic.Height      = 100;
		pic.BorderStyle = 1;  // FixedSingle
		pic.SizeMode    = 4;  // Zoom

		tabControls.AddChild(lbl);
		tabControls.AddChild(link);
		tabControls.AddChild(lb);
		tabControls.AddChild(lblStatus);
		tabControls.AddChild(pb);
		tabControls.AddChild(btnStep);
		tabControls.AddChild(pic);

		tabs.AddChild(tabDgv);
		tabs.AddChild(tabSplit);
		tabs.AddChild(tabLegacy);
		tabs.AddChild(tabControls);

		// --- Bottom panel with CheckBox, RadioButtons, Button ---
		CppWinForms::Panel bottomPanel;
		bottomPanel.Height = 45;
		bottomPanel.Dock   = 2;  // Bottom

		CppWinForms::CheckBox checkBox;
		checkBox.Text     = L"\u542F\u7528\u901A\u77E5";  // 启用通知
		checkBox.AutoSize = true;
		checkBox.Checked  = true;
		checkBox.Left     = 12;
		checkBox.Top      = 12;

		CppWinForms::GroupBox radioGroup;
		radioGroup.Text   = L"\u6A21\u5F0F";  // 模式
		radioGroup.Left   = 140;
		radioGroup.Top    = 0;
		radioGroup.Width  = 220;
		radioGroup.Height = 42;

		CppWinForms::RadioButton radioA;
		radioA.Text     = L"\u6807\u51C6";  // 标准
		radioA.AutoSize = true;
		radioA.Checked  = true;
		radioA.Left     = 12;
		radioA.Top      = 16;

		CppWinForms::RadioButton radioB;
		radioB.Text     = L"\u9AD8\u7EA7";  // 高级
		radioB.AutoSize = true;
		radioB.Left     = 80;
		radioB.Top      = 16;

		radioGroup.AddChild(radioA);
		radioGroup.AddChild(radioB);

		CppWinForms::Button button;
		button.Text   = L"\u70B9\u51FB\u5F39\u7A97";
		button.Width  = 100;
		button.Height = 28;
		button.Left   = 380;
		button.Top    = 8;

		button.OnClick([&]() {
			std::wstring msg = L"\u901A\u77E5: ";
			msg += checkBox.Checked ? L"\u5F00" : L"\u5173";
			msg += L"  \u6A21\u5F0F: ";
			msg += radioA.Checked ? L"\u6807\u51C6" : L"\u9AD8\u7EA7";
			MessageBoxW(nullptr, msg.c_str(),
				L"\u72B6\u6001", MB_OK | MB_ICONINFORMATION);
		});

		bottomPanel.AddChild(checkBox);
		bottomPanel.AddChild(radioGroup);
		bottomPanel.AddChild(button);

		// Add controls — order matters for dock layout:
		// last-added is front of Z-order, docked first
		form.AddChild(tabs);           // Fill  (back)
		form.AddChild(bottomPanel);    // Bottom
		form.AddChild(toolStrip);      // Top (below menu)
		form.AddChild(menu);           // Top   (front)
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
