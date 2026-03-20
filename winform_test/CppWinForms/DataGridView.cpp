#include "DataGridView.h"

namespace CppWinForms
{

DataGridView::DataGridView()
{
	InitControl(L"System.Windows.Forms.DataGridView");
}

void DataGridView::AddColumnImpl(
	const wchar_t* typeName, const wchar_t* headerText, int width)
{
	mscorlib::_ObjectHandlePtr handle =
		__Internal::AppDomain()->CreateInstanceFrom(
			_bstr_t(kFormsAssemblyPath), _bstr_t(typeName));
	_variant_t col = handle->Unwrap();
	mscorlib::_TypePtr colType = __Internal::GetTypeFromVariant(col);

	_variant_t textVal(headerText);
	SAFEARRAY* a = __Internal::MakeArgArray({ &textVal });
	colType->InvokeMember_3(
		_bstr_t(L"HeaderText"), __Internal::kSetProperty,
		nullptr, col, a);
	SafeArrayDestroy(a);

	if (width > 0)
	{
		_variant_t widthVal(static_cast<long>(width));
		a = __Internal::MakeArgArray({ &widthVal });
		colType->InvokeMember_3(
			_bstr_t(L"Width"), __Internal::kSetProperty,
			nullptr, col, a);
		SafeArrayDestroy(a);
	}

	_variant_t columns = type_->InvokeMember_3(
		_bstr_t(L"Columns"), __Internal::kGetProperty,
		nullptr, variant_, nullptr);
	mscorlib::_TypePtr colsType = __Internal::GetTypeFromVariant(columns);

	a = __Internal::MakeArgArray({ &col });
	colsType->InvokeMember_3(
		_bstr_t(L"Add"), __Internal::kInvokeMethod,
		nullptr, columns, a);
	SafeArrayDestroy(a);
}

void DataGridView::AddTextColumn(const wchar_t* headerText, int width)
{
	AddColumnImpl(
		L"System.Windows.Forms.DataGridViewTextBoxColumn",
		headerText, width);
}

void DataGridView::AddCheckBoxColumn(const wchar_t* headerText, int width)
{
	AddColumnImpl(
		L"System.Windows.Forms.DataGridViewCheckBoxColumn",
		headerText, width);
}

void DataGridView::AddRow(std::initializer_list<const wchar_t*> cells)
{
	_variant_t rows = type_->InvokeMember_3(
		_bstr_t(L"Rows"), __Internal::kGetProperty,
		nullptr, variant_, nullptr);
	mscorlib::_TypePtr rowsType = __Internal::GetTypeFromVariant(rows);

	SAFEARRAY* sa = SafeArrayCreateVector(
		VT_VARIANT, 0, static_cast<ULONG>(cells.size()));
	LONG idx = 0;
	for (const wchar_t* text : cells)
	{
		_variant_t v(text);
		SafeArrayPutElement(sa, &idx, &v);
		++idx;
	}

	rowsType->InvokeMember_3(
		_bstr_t(L"Add"), __Internal::kInvokeMethod,
		nullptr, rows, sa);
	SafeArrayDestroy(sa);
}

void DataGridView::SetVirtualMode(int rowCount, CellValueProvider provider)
{
	cellProvider_ = std::make_unique<CellValueProvider>(std::move(provider));
	__Internal::AttachDgvVirtualMode(
		variant_, rowCount, cellProvider_.get());
}

} // namespace CppWinForms
