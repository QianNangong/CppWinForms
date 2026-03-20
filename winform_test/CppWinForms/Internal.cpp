#include "Internal.h"

namespace CppWinForms::__Internal
{

static mscorlib::_AppDomainPtr g_appDomain;
static mscorlib::_AssemblyPtr  g_formsAssembly;

mscorlib::_AppDomainPtr& AppDomain()    { return g_appDomain; }
mscorlib::_AssemblyPtr&  FormsAssembly(){ return g_formsAssembly; }

// ---------------------------------------------------------------------------
// IDispatch helpers
// ---------------------------------------------------------------------------
HRESULT GetDispId(IDispatch* dispatch, const wchar_t* name, DISPID* dispId)
{
	LPOLESTR memberName = const_cast<LPOLESTR>(name);
	return dispatch->GetIDsOfNames(
		IID_NULL, &memberName, 1, LOCALE_USER_DEFAULT, dispId);
}

HRESULT GetProperty(IDispatch* dispatch, const wchar_t* name, VARIANT* result)
{
	DISPID dispId = DISPID_UNKNOWN;
	HRESULT hr = GetDispId(dispatch, name, &dispId);
	if (FAILED(hr)) return hr;
	DISPPARAMS params{};
	return dispatch->Invoke(
		dispId, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_PROPERTYGET, &params, result, nullptr, nullptr);
}

HRESULT PutProperty(IDispatch* dispatch, const wchar_t* name, const VARIANT& value)
{
	DISPID dispId = DISPID_UNKNOWN;
	HRESULT hr = GetDispId(dispatch, name, &dispId);
	if (FAILED(hr)) return hr;
	DISPID namedArg = DISPID_PROPERTYPUT;
	DISPPARAMS params{};
	params.rgvarg            = const_cast<VARIANT*>(&value);
	params.rgdispidNamedArgs = &namedArg;
	params.cArgs             = 1;
	params.cNamedArgs        = 1;
	return dispatch->Invoke(
		dispId, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_PROPERTYPUT, &params, nullptr, nullptr, nullptr);
}

// ---------------------------------------------------------------------------
// Managed reflection helpers
// ---------------------------------------------------------------------------
mscorlib::_TypePtr GetTypeFromVariant(const _variant_t& obj)
{
	IUnknown* punk = nullptr;
	if (obj.vt == VT_DISPATCH && obj.pdispVal != nullptr)
		punk = obj.pdispVal;
	else if (obj.vt == VT_UNKNOWN && obj.punkVal != nullptr)
		punk = obj.punkVal;
	if (punk == nullptr) return nullptr;

	mscorlib::_ObjectPtr managedObj;
	HRESULT hr = punk->QueryInterface(
		__uuidof(mscorlib::_Object),
		reinterpret_cast<void**>(&managedObj));
	if (FAILED(hr) || managedObj == nullptr) return nullptr;
	return managedObj->GetType();
}

SAFEARRAY* MakeArgArray(std::initializer_list<const _variant_t*> args)
{
	SAFEARRAY* sa = SafeArrayCreateVector(
		VT_VARIANT, 0, static_cast<ULONG>(args.size()));
	LONG idx = 0;
	for (const _variant_t* arg : args)
	{
		SafeArrayPutElement(sa, &idx,
			const_cast<VARIANT*>(static_cast<const VARIANT*>(arg)));
		++idx;
	}
	return sa;
}

void AddToCollection(
	const _variant_t& owner,
	const wchar_t* collectionProp,
	const _variant_t& item)
{
	mscorlib::_TypePtr ownerType = GetTypeFromVariant(owner);
	_variant_t collection = ownerType->InvokeMember_3(
		_bstr_t(collectionProp), kGetProperty,
		nullptr, owner, nullptr);
	mscorlib::_TypePtr collType = GetTypeFromVariant(collection);

	_variant_t itemCopy(item);
	SAFEARRAY* args = MakeArgArray({ &itemCopy });
	collType->InvokeMember_3(
		_bstr_t(L"Add"), kInvokeMethod,
		nullptr, collection, args);
	SafeArrayDestroy(args);
}

// ---------------------------------------------------------------------------
// EventBridge — native thunk + managed shim
// ---------------------------------------------------------------------------
static mscorlib::_TypePtr g_eventBridgeType;

static void __stdcall EventThunk(void* context)
{
	auto* fn = static_cast<std::function<void()>*>(context);
	(*fn)();
}

void InitEventBridge()
{
	// Generic shim: accepts native __stdcall callback + context pointer.
	// Uses delegate parameter contravariance so a single (object, EventArgs)
	// proxy method works for EventHandler, MouseEventHandler, etc.
	static constexpr wchar_t kSource[] =
		L"using System;using System.Runtime.InteropServices;using System.Windows.Forms;"
		L"public static class EventBridge{"
		L"[UnmanagedFunctionPointer(CallingConvention.StdCall)]"
		L"delegate void Fn(IntPtr c);"
		L"class P{Action a;public P(Action x){a=x;}"
		L"public void I(object s,EventArgs e){a();}}"
		L"public static void Add(object c,string e,long fn,long ctx){"
		L"var d=(Fn)Marshal.GetDelegateForFunctionPointer(new IntPtr(fn),typeof(Fn));"
		L"var p=new P(()=>d(new IntPtr(ctx)));var ei=c.GetType().GetEvent(e);"
		L"ei.AddEventHandler(c,Delegate.CreateDelegate(ei.EventHandlerType,p,typeof(P).GetMethod(\"I\")));}"
		L"[UnmanagedFunctionPointer(CallingConvention.StdCall)]"
		L"delegate void RowFn(int i,IntPtr b,int c,IntPtr x);"
		L"public static void SetVirtual(ListView lv,int count,int cols,long fn,long ctx){"
		L"var d=(RowFn)Marshal.GetDelegateForFunctionPointer(new IntPtr(fn),typeof(RowFn));"
		L"lv.VirtualMode=true;lv.VirtualListSize=count;"
		L"lv.RetrieveVirtualItem+=(s,e)=>{"
		L"int sz=IntPtr.Size*cols;IntPtr buf=Marshal.AllocCoTaskMem(sz);"
		L"for(int i=0;i<cols;i++)Marshal.WriteIntPtr(buf,i*IntPtr.Size,IntPtr.Zero);"
		L"d(e.ItemIndex,buf,cols,new IntPtr(ctx));"
		L"var t=new string[cols];"
		L"for(int i=0;i<cols;i++){IntPtr b=Marshal.ReadIntPtr(buf,i*IntPtr.Size);"
		L"t[i]=b!=IntPtr.Zero?Marshal.PtrToStringBSTR(b):\"\";"
		L"if(b!=IntPtr.Zero)Marshal.FreeBSTR(b);}"
		L"Marshal.FreeCoTaskMem(buf);e.Item=new ListViewItem(t);};}"
		L"[UnmanagedFunctionPointer(CallingConvention.StdCall)]"
		L"delegate IntPtr CellFn(int r,int c,IntPtr x);"
		L"public static void SetDgvVirtual(DataGridView g,int rows,long fn,long ctx){"
		L"var d=(CellFn)Marshal.GetDelegateForFunctionPointer(new IntPtr(fn),typeof(CellFn));"
		L"g.VirtualMode=true;g.RowCount=rows;"
		L"g.CellValueNeeded+=(s,e)=>{"
		L"IntPtr b=d(e.RowIndex,e.ColumnIndex,new IntPtr(ctx));"
		L"if(b!=IntPtr.Zero){e.Value=Marshal.PtrToStringBSTR(b);Marshal.FreeBSTR(b);}"
		L"};}"
		L"}";

	auto& appDomain = AppDomain();

	mscorlib::_ObjectHandlePtr paramsHandle = appDomain->CreateInstanceFrom(
		_bstr_t(kSystemDllPath),
		_bstr_t(L"System.CodeDom.Compiler.CompilerParameters"));
	_variant_t paramsObj = paramsHandle->Unwrap();
	mscorlib::_TypePtr paramsType = GetTypeFromVariant(paramsObj);

	{
		_variant_t trueVal(true);
		SAFEARRAY* a = MakeArgArray({ &trueVal });
		paramsType->InvokeMember_3(
			_bstr_t(L"GenerateInMemory"), kSetProperty, nullptr, paramsObj, a);
		SafeArrayDestroy(a);
	}

	_variant_t refsCol = paramsType->InvokeMember_3(
		_bstr_t(L"ReferencedAssemblies"), kGetProperty, nullptr, paramsObj, nullptr);
	mscorlib::_TypePtr refsType = GetTypeFromVariant(refsCol);
	for (const wchar_t* ref :
		{ L"System.dll", L"System.Drawing.dll", L"System.Windows.Forms.dll" })
	{
		_variant_t refStr(ref);
		SAFEARRAY* a = MakeArgArray({ &refStr });
		refsType->InvokeMember_3(
			_bstr_t(L"Add"), kInvokeMethod, nullptr, refsCol, a);
		SafeArrayDestroy(a);
	}

	mscorlib::_ObjectHandlePtr provHandle = appDomain->CreateInstanceFrom(
		_bstr_t(kSystemDllPath),
		_bstr_t(L"Microsoft.CSharp.CSharpCodeProvider"));
	_variant_t provObj = provHandle->Unwrap();
	mscorlib::_TypePtr provType = GetTypeFromVariant(provObj);

	_variant_t srcStr(kSource);
	SAFEARRAY* compileArgs = MakeArgArray({ &paramsObj, &srcStr });
	_variant_t resultObj = provType->InvokeMember_3(
		_bstr_t(L"CompileAssemblyFromSource"), kInvokeMethod,
		nullptr, provObj, compileArgs);
	SafeArrayDestroy(compileArgs);

	mscorlib::_TypePtr resultType = GetTypeFromVariant(resultObj);
	_variant_t errorsObj = resultType->InvokeMember_3(
		_bstr_t(L"Errors"), kGetProperty, nullptr, resultObj, nullptr);
	mscorlib::_TypePtr errorsType = GetTypeFromVariant(errorsObj);
	_variant_t hasErrors = errorsType->InvokeMember_3(
		_bstr_t(L"HasErrors"), kGetProperty, nullptr, errorsObj, nullptr);
	if (hasErrors.boolVal == VARIANT_TRUE)
	{
		std::printf("EventBridge compilation failed.\n");
		return;
	}

	_variant_t asmObj = resultType->InvokeMember_3(
		_bstr_t(L"CompiledAssembly"), kGetProperty, nullptr, resultObj, nullptr);
	mscorlib::_TypePtr asmType = GetTypeFromVariant(asmObj);
	_variant_t typeName(L"EventBridge");
	SAFEARRAY* gtArgs = MakeArgArray({ &typeName });
	_variant_t bridgeTypeVar = asmType->InvokeMember_3(
		_bstr_t(L"GetType"), kInvokeMethod, nullptr, asmObj, gtArgs);
	SafeArrayDestroy(gtArgs);

	IUnknown* punk = (bridgeTypeVar.vt == VT_UNKNOWN) ? bridgeTypeVar.punkVal :
		(bridgeTypeVar.vt == VT_DISPATCH)
			? static_cast<IUnknown*>(bridgeTypeVar.pdispVal) : nullptr;
	punk->QueryInterface(
		__uuidof(mscorlib::_Type),
		reinterpret_cast<void**>(&g_eventBridgeType));
}

void AttachEvent(
	const _variant_t& control,
	const wchar_t* eventName,
	std::function<void()>* callback)
{
	_variant_t ctrlVar(control);
	_variant_t nameVar(eventName);
	_variant_t fnVar(static_cast<long long>(
		reinterpret_cast<intptr_t>(&EventThunk)));
	_variant_t ctxVar(static_cast<long long>(
		reinterpret_cast<intptr_t>(callback)));

	SAFEARRAY* args = MakeArgArray({ &ctrlVar, &nameVar, &fnVar, &ctxVar });
	g_eventBridgeType->InvokeMember_3(
		_bstr_t(L"Add"), kStaticInvoke, nullptr, _variant_t(), args);
	SafeArrayDestroy(args);
}

// ---------------------------------------------------------------------------
// Virtual ListView support
// ---------------------------------------------------------------------------
static void __stdcall VirtualItemThunk(
	int index, BSTR* texts, int colCount, void* context)
{
	auto* provider = static_cast<
		std::function<std::vector<std::wstring>(int)>*>(context);
	auto row = (*provider)(index);
	for (int i = 0; i < colCount && i < static_cast<int>(row.size()); i++)
		texts[i] = SysAllocString(row[i].c_str());
}

void AttachVirtualMode(
	const _variant_t& listView,
	int itemCount,
	int columnCount,
	std::function<std::vector<std::wstring>(int)>* provider)
{
	_variant_t lvVar(listView);
	_variant_t countVar(static_cast<long>(itemCount));
	_variant_t colsVar(static_cast<long>(columnCount));
	_variant_t fnVar(static_cast<long long>(
		reinterpret_cast<intptr_t>(&VirtualItemThunk)));
	_variant_t ctxVar(static_cast<long long>(
		reinterpret_cast<intptr_t>(provider)));

	SAFEARRAY* args = MakeArgArray(
		{ &lvVar, &countVar, &colsVar, &fnVar, &ctxVar });
	g_eventBridgeType->InvokeMember_3(
		_bstr_t(L"SetVirtual"), kStaticInvoke, nullptr, _variant_t(), args);
	SafeArrayDestroy(args);
}

// ---------------------------------------------------------------------------
// DataGridView virtual mode support
// ---------------------------------------------------------------------------
static BSTR __stdcall DgvCellThunk(int row, int col, void* context)
{
	auto* provider = static_cast<
		std::function<std::wstring(int, int)>*>(context);
	auto text = (*provider)(row, col);
	return SysAllocString(text.c_str());
}

void AttachDgvVirtualMode(
	const _variant_t& dgv,
	int rowCount,
	std::function<std::wstring(int, int)>* provider)
{
	_variant_t dgvVar(dgv);
	_variant_t rowsVar(static_cast<long>(rowCount));
	_variant_t fnVar(static_cast<long long>(
		reinterpret_cast<intptr_t>(&DgvCellThunk)));
	_variant_t ctxVar(static_cast<long long>(
		reinterpret_cast<intptr_t>(provider)));

	SAFEARRAY* args = MakeArgArray(
		{ &dgvVar, &rowsVar, &fnVar, &ctxVar });
	g_eventBridgeType->InvokeMember_3(
		_bstr_t(L"SetDgvVirtual"), kStaticInvoke,
		nullptr, _variant_t(), args);
	SafeArrayDestroy(args);
}

} // namespace CppWinForms::__Internal
