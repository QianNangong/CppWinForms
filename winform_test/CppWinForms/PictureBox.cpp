#include "PictureBox.h"

namespace CppWinForms
{

PictureBox::PictureBox()
{
	InitControl(L"System.Windows.Forms.PictureBox");
}

void PictureBox::LoadFromFile(const wchar_t* path)
{
	__Internal::PutProperty(dispatch_, L"ImageLocation", _variant_t(path));
}

} // namespace CppWinForms
