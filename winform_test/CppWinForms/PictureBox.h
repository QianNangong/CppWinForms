#pragma once

#include "Control.h"

namespace CppWinForms
{

// SizeMode (PictureBoxSizeMode):
//   Normal=0, StretchImage=1, AutoSize=2, CenterImage=3, Zoom=4
// BorderStyle: None=0, FixedSingle=1, Fixed3D=2
class PictureBox : public Control
{
public:
	__declspec(property(get = get_SizeMode, put = put_SizeMode))
	long SizeMode;
	__declspec(property(get = get_BorderStyle, put = put_BorderStyle))
	long BorderStyle;

	long get_SizeMode() const    { return GetLongProp(L"SizeMode"); }
	void put_SizeMode(long v)    { PutLongProp(L"SizeMode", v); }
	long get_BorderStyle() const { return GetLongProp(L"BorderStyle"); }
	void put_BorderStyle(long v) { PutLongProp(L"BorderStyle", v); }

	PictureBox();

	void LoadFromFile(const wchar_t* path);
};

} // namespace CppWinForms
