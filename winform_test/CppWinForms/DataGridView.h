#pragma once

#include "Control.h"
#include <string>

namespace CppWinForms
{

using CellValueProvider = std::function<std::wstring(int row, int col)>;

// SelectionMode: CellSelect=0, FullRowSelect=1, FullColumnSelect=2,
//                RowHeaderSelect=3, ColumnHeaderSelect=4
// AutoSizeColumnsMode: None=0, ColumnHeader=2, AllCellsExceptHeader=4,
//                      AllCells=6, DisplayedCells=10, Fill=16
class DataGridView : public Control
{
public:
	__declspec(property(get = get_ReadOnly, put = put_ReadOnly))
	bool ReadOnly;
	__declspec(property(get = get_AllowUserToAddRows, put = put_AllowUserToAddRows))
	bool AllowUserToAddRows;
	__declspec(property(get = get_AllowUserToDeleteRows, put = put_AllowUserToDeleteRows))
	bool AllowUserToDeleteRows;
	__declspec(property(get = get_RowHeadersVisible, put = put_RowHeadersVisible))
	bool RowHeadersVisible;
	__declspec(property(get = get_AutoSizeColumnsMode, put = put_AutoSizeColumnsMode))
	long AutoSizeColumnsMode;
	__declspec(property(get = get_SelectionMode, put = put_SelectionMode))
	long SelectionMode;
	__declspec(property(get = get_ColumnCount, put = put_ColumnCount))
	long ColumnCount;
	__declspec(property(get = get_RowCount, put = put_RowCount))
	long RowCount;

	bool get_ReadOnly() const               { return GetBoolProp(L"ReadOnly"); }
	void put_ReadOnly(bool v)               { PutBoolProp(L"ReadOnly", v); }
	bool get_AllowUserToAddRows() const     { return GetBoolProp(L"AllowUserToAddRows"); }
	void put_AllowUserToAddRows(bool v)     { PutBoolProp(L"AllowUserToAddRows", v); }
	bool get_AllowUserToDeleteRows() const  { return GetBoolProp(L"AllowUserToDeleteRows"); }
	void put_AllowUserToDeleteRows(bool v)  { PutBoolProp(L"AllowUserToDeleteRows", v); }
	bool get_RowHeadersVisible() const      { return GetBoolProp(L"RowHeadersVisible"); }
	void put_RowHeadersVisible(bool v)      { PutBoolProp(L"RowHeadersVisible", v); }
	long get_AutoSizeColumnsMode() const    { return GetLongProp(L"AutoSizeColumnsMode"); }
	void put_AutoSizeColumnsMode(long v)    { PutLongProp(L"AutoSizeColumnsMode", v); }
	long get_SelectionMode() const          { return GetLongProp(L"SelectionMode"); }
	void put_SelectionMode(long v)          { PutLongProp(L"SelectionMode", v); }
	long get_ColumnCount() const            { return GetLongProp(L"ColumnCount"); }
	void put_ColumnCount(long v)            { PutLongProp(L"ColumnCount", v); }
	long get_RowCount() const               { return GetLongProp(L"RowCount"); }
	void put_RowCount(long v)               { PutLongProp(L"RowCount", v); }

	DataGridView();

	void AddTextColumn(const wchar_t* headerText, int width = -1);
	void AddCheckBoxColumn(const wchar_t* headerText, int width = -1);
	void AddRow(std::initializer_list<const wchar_t*> cells);
	void SetVirtualMode(int rowCount, CellValueProvider provider);

private:
	void AddColumnImpl(const wchar_t* typeName, const wchar_t* headerText, int width);
	std::unique_ptr<CellValueProvider> cellProvider_;
};

} // namespace CppWinForms
