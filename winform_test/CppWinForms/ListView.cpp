#include "ListView.h"

namespace CppWinForms
{

ListView::ListView()
{
	InitControl(L"System.Windows.Forms.ListView");
}

bool ListView::get_FullRowSelect() const
{
	_variant_t r;
	__Internal::GetProperty(dispatch_, L"FullRowSelect", &r);
	return r.boolVal == VARIANT_TRUE;
}

void ListView::put_FullRowSelect(bool v)
{
	__Internal::PutProperty(dispatch_, L"FullRowSelect",
		_variant_t(v ? VARIANT_TRUE : VARIANT_FALSE, VT_BOOL));
}

bool ListView::get_GridLines() const
{
	_variant_t r;
	__Internal::GetProperty(dispatch_, L"GridLines", &r);
	return r.boolVal == VARIANT_TRUE;
}

void ListView::put_GridLines(bool v)
{
	__Internal::PutProperty(dispatch_, L"GridLines",
		_variant_t(v ? VARIANT_TRUE : VARIANT_FALSE, VT_BOOL));
}

void ListView::AddColumn(const wchar_t* text, long width)
{
	_variant_t columns = type_->InvokeMember_3(
		_bstr_t(L"Columns"), __Internal::kGetProperty,
		nullptr, variant_, nullptr);
	mscorlib::_TypePtr colsType = __Internal::GetTypeFromVariant(columns);

	_variant_t textVar(text);
	_variant_t widthVar(width);
	SAFEARRAY* args = __Internal::MakeArgArray({ &textVar, &widthVar });
	colsType->InvokeMember_3(
		_bstr_t(L"Add"), __Internal::kInvokeMethod,
		nullptr, columns, args);
	SafeArrayDestroy(args);
}

void ListView::AddRow(std::initializer_list<const wchar_t*> cells)
{
	auto it = cells.begin();
	if (it == cells.end()) return;

	// Items.Add(firstCell) -> returns ListViewItem
	_variant_t items = type_->InvokeMember_3(
		_bstr_t(L"Items"), __Internal::kGetProperty,
		nullptr, variant_, nullptr);
	mscorlib::_TypePtr itemsType = __Internal::GetTypeFromVariant(items);

	_variant_t firstCell(*it);
	SAFEARRAY* args = __Internal::MakeArgArray({ &firstCell });
	_variant_t lvItem = itemsType->InvokeMember_3(
		_bstr_t(L"Add"), __Internal::kInvokeMethod,
		nullptr, items, args);
	SafeArrayDestroy(args);

	// SubItems.Add for remaining cells
	mscorlib::_TypePtr lvItemType = __Internal::GetTypeFromVariant(lvItem);
	_variant_t subItems = lvItemType->InvokeMember_3(
		_bstr_t(L"SubItems"), __Internal::kGetProperty,
		nullptr, lvItem, nullptr);
	mscorlib::_TypePtr subType = __Internal::GetTypeFromVariant(subItems);

	for (++it; it != cells.end(); ++it)
	{
		_variant_t cellVal(*it);
		SAFEARRAY* a = __Internal::MakeArgArray({ &cellVal });
		subType->InvokeMember_3(
			_bstr_t(L"Add"), __Internal::kInvokeMethod,
			nullptr, subItems, a);
		SafeArrayDestroy(a);
	}
}

} // namespace CppWinForms
