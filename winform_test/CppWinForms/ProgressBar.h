#pragma once

#include "Control.h"

namespace CppWinForms
{

// Style (ProgressBarStyle): Blocks=0, Continuous=1, Marquee=2
class ProgressBar : public Control
{
public:
	__declspec(property(get = get_Value, put = put_Value))
	long Value;
	__declspec(property(get = get_Minimum, put = put_Minimum))
	long Minimum;
	__declspec(property(get = get_Maximum, put = put_Maximum))
	long Maximum;
	__declspec(property(get = get_Step, put = put_Step))
	long Step;
	__declspec(property(get = get_Style, put = put_Style))
	long Style;

	long get_Value() const   { return GetLongProp(L"Value"); }
	void put_Value(long v)   { PutLongProp(L"Value", v); }
	long get_Minimum() const { return GetLongProp(L"Minimum"); }
	void put_Minimum(long v) { PutLongProp(L"Minimum", v); }
	long get_Maximum() const { return GetLongProp(L"Maximum"); }
	void put_Maximum(long v) { PutLongProp(L"Maximum", v); }
	long get_Step() const    { return GetLongProp(L"Step"); }
	void put_Step(long v)    { PutLongProp(L"Step", v); }
	long get_Style() const   { return GetLongProp(L"Style"); }
	void put_Style(long v)   { PutLongProp(L"Style", v); }

	ProgressBar();

	void PerformStep();
	void Increment(long value);
};

} // namespace CppWinForms
