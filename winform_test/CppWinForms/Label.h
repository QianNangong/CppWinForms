#pragma once

#include "Control.h"

namespace CppWinForms
{

// TextAlign (ContentAlignment):
//   TopLeft=1, TopCenter=2, TopRight=4,
//   MiddleLeft=16, MiddleCenter=32, MiddleRight=64,
//   BottomLeft=256, BottomCenter=512, BottomRight=1024
class Label : public Control
{
public:
	__declspec(property(get = get_AutoSize, put = put_AutoSize))
	bool AutoSize;
	__declspec(property(get = get_TextAlign, put = put_TextAlign))
	long TextAlign;
	__declspec(property(get = get_BorderStyle, put = put_BorderStyle))
	long BorderStyle;

	bool get_AutoSize() const   { return GetBoolProp(L"AutoSize"); }
	void put_AutoSize(bool v)   { PutBoolProp(L"AutoSize", v); }
	long get_TextAlign() const  { return GetLongProp(L"TextAlign"); }
	void put_TextAlign(long v)  { PutLongProp(L"TextAlign", v); }
	long get_BorderStyle() const{ return GetLongProp(L"BorderStyle"); }
	void put_BorderStyle(long v){ PutLongProp(L"BorderStyle", v); }

	Label();
};

} // namespace CppWinForms
