#pragma once

#include "Control.h"

namespace CppWinForms
{

// Orientation: Horizontal=0, Vertical=1
// FixedPanel:  None=0, Panel1=1, Panel2=2
class SplitContainer : public Control
{
public:
	__declspec(property(get = get_Orientation, put = put_Orientation))
	long Orientation;
	__declspec(property(get = get_SplitterDistance, put = put_SplitterDistance))
	long SplitterDistance;
	__declspec(property(get = get_SplitterWidth, put = put_SplitterWidth))
	long SplitterWidth;
	__declspec(property(get = get_FixedPanel, put = put_FixedPanel))
	long FixedPanel;

	long get_Orientation() const     { return GetLongProp(L"Orientation"); }
	void put_Orientation(long v)     { PutLongProp(L"Orientation", v); }
	long get_SplitterDistance() const{ return GetLongProp(L"SplitterDistance"); }
	void put_SplitterDistance(long v){ PutLongProp(L"SplitterDistance", v); }
	long get_SplitterWidth() const   { return GetLongProp(L"SplitterWidth"); }
	void put_SplitterWidth(long v)   { PutLongProp(L"SplitterWidth", v); }
	long get_FixedPanel() const      { return GetLongProp(L"FixedPanel"); }
	void put_FixedPanel(long v)      { PutLongProp(L"FixedPanel", v); }

	SplitContainer();

	Control& Panel1();
	Control& Panel2();

private:
	Control panel1_;
	Control panel2_;
	bool panelsInit_ = false;
	void EnsurePanels();
};

} // namespace CppWinForms
