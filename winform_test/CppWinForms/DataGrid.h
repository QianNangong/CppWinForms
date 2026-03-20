#pragma once

#include "Control.h"
#include <string>
#include <vector>

namespace CppWinForms
{

class DataGrid : public Control
{
public:
	__declspec(property(get = get_ReadOnly, put = put_ReadOnly))
	bool ReadOnly;
	__declspec(property(get = get_CaptionVisible, put = put_CaptionVisible))
	bool CaptionVisible;

	bool get_ReadOnly() const       { return GetBoolProp(L"ReadOnly"); }
	void put_ReadOnly(bool v)       { PutBoolProp(L"ReadOnly", v); }
	bool get_CaptionVisible() const { return GetBoolProp(L"CaptionVisible"); }
	void put_CaptionVisible(bool v) { PutBoolProp(L"CaptionVisible", v); }

	DataGrid();

	void SetSimpleData(
		std::initializer_list<const wchar_t*> columnNames,
		std::initializer_list<std::initializer_list<const wchar_t*>> rows);
};

} // namespace CppWinForms
