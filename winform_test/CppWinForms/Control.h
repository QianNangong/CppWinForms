#pragma once

#include "Internal.h"
#include <functional>
#include <memory>
#include <vector>

namespace CppWinForms
{

void Initialize(mscorlib::_AppDomainPtr& appDomain);

class Control
{
public:
	__declspec(property(get = get_Text, put = put_Text))
	_bstr_t Text;
	__declspec(property(get = get_Width, put = put_Width))
	long Width;
	__declspec(property(get = get_Height, put = put_Height))
	long Height;
	__declspec(property(get = get_Left, put = put_Left))
	long Left;
	__declspec(property(get = get_Top, put = put_Top))
	long Top;
	__declspec(property(get = get_Anchor, put = put_Anchor))
	long Anchor;
	__declspec(property(get = get_Visible, put = put_Visible))
	bool Visible;

	_bstr_t get_Text() const;
	void    put_Text(const wchar_t* value);
	long    get_Width() const   { return GetLongProp(L"Width"); }
	void    put_Width(long v)   { PutLongProp(L"Width", v); }
	long    get_Height() const  { return GetLongProp(L"Height"); }
	void    put_Height(long v)  { PutLongProp(L"Height", v); }
	long    get_Left() const    { return GetLongProp(L"Left"); }
	void    put_Left(long v)    { PutLongProp(L"Left", v); }
	long    get_Top() const     { return GetLongProp(L"Top"); }
	void    put_Top(long v)     { PutLongProp(L"Top", v); }
	long    get_Anchor() const  { return GetLongProp(L"Anchor"); }
	void    put_Anchor(long v)  { PutLongProp(L"Anchor", v); }
	bool    get_Visible() const;
	void    put_Visible(bool v);

	void AddEventHandler(const wchar_t* eventName, std::function<void()> callback);

	IDispatch*             dispatch() const { return dispatch_; }
	const _variant_t&      variant() const  { return variant_; }
	mscorlib::_TypePtr     managedType() const { return type_; }

protected:
	IDispatch*         dispatch_ = nullptr;
	_variant_t         variant_;
	mscorlib::_TypePtr type_;

	Control() = default;
	void InitControl(const wchar_t* typeName);

	long GetLongProp(const wchar_t* name) const;
	void PutLongProp(const wchar_t* name, long value);

private:
	std::vector<std::unique_ptr<std::function<void()>>> callbacks_;
};

} // namespace CppWinForms
