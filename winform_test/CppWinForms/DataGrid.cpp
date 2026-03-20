#include "DataGrid.h"

namespace CppWinForms
{

DataGrid::DataGrid()
{
	InitControl(L"System.Windows.Forms.DataGrid");
}

void DataGrid::SetSimpleData(
	std::initializer_list<const wchar_t*> columnNames,
	std::initializer_list<std::initializer_list<const wchar_t*>> rows)
{
	auto& appDomain = __Internal::AppDomain();

	// Create DataTable
	mscorlib::_ObjectHandlePtr tableHandle = appDomain->CreateInstanceFrom(
		_bstr_t(kDataDllPath),
		_bstr_t(L"System.Data.DataTable"));
	_variant_t tableObj = tableHandle->Unwrap();
	mscorlib::_TypePtr tableType = __Internal::GetTypeFromVariant(tableObj);

	// Add columns: table.Columns.Add(name)
	_variant_t columnsObj = tableType->InvokeMember_3(
		_bstr_t(L"Columns"), __Internal::kGetProperty,
		nullptr, tableObj, nullptr);
	mscorlib::_TypePtr columnsType = __Internal::GetTypeFromVariant(columnsObj);

	for (const wchar_t* name : columnNames)
	{
		_variant_t nameVar(name);
		SAFEARRAY* a = __Internal::MakeArgArray({ &nameVar });
		columnsType->InvokeMember_3(
			_bstr_t(L"Add"), __Internal::kInvokeMethod,
			nullptr, columnsObj, a);
		SafeArrayDestroy(a);
	}

	// Add rows: table.NewRow() -> set Item[i] -> table.Rows.Add(row)
	_variant_t rowsObj = tableType->InvokeMember_3(
		_bstr_t(L"Rows"), __Internal::kGetProperty,
		nullptr, tableObj, nullptr);
	mscorlib::_TypePtr rowsType = __Internal::GetTypeFromVariant(rowsObj);

	for (const auto& row : rows)
	{
		_variant_t newRow = tableType->InvokeMember_3(
			_bstr_t(L"NewRow"), __Internal::kInvokeMethod,
			nullptr, tableObj, nullptr);
		mscorlib::_TypePtr rowType = __Internal::GetTypeFromVariant(newRow);

		long colIdx = 0;
		for (const wchar_t* cell : row)
		{
			_variant_t idxVar(colIdx);
			_variant_t cellVar(cell);
			SAFEARRAY* a = __Internal::MakeArgArray({ &idxVar, &cellVar });
			rowType->InvokeMember_3(
				_bstr_t(L"Item"), __Internal::kSetProperty,
				nullptr, newRow, a);
			SafeArrayDestroy(a);
			++colIdx;
		}

		SAFEARRAY* a = __Internal::MakeArgArray({ &newRow });
		rowsType->InvokeMember_3(
			_bstr_t(L"Add"), __Internal::kInvokeMethod,
			nullptr, rowsObj, a);
		SafeArrayDestroy(a);
	}

	// DataGrid.DataSource = dataTable
	__Internal::PutProperty(dispatch_, L"DataSource", tableObj);
}

} // namespace CppWinForms
