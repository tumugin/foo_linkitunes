#pragma once
#include "iTunesHeader.h"
class CITunesEventSink :public _IiTunesEvents{
private:
	long m_dwRefCount;
	// Pointer to type information.
	ITypeInfo* m_pITypeInfo;
public:
	CITunesEventSink();
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppvObject);
	HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo);
	HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT itinfo, LCID lcid, ITypeInfo** pptinfo);
	HRESULT __stdcall GetIDsOfNames(const IID& iid, OLECHAR** arrayNames, UINT countNames,LCID,DISPID* arrayDispIDs);
	HRESULT STDMETHODCALLTYPE Invoke(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult, EXCEPINFO* pexcepinfo, UINT* puArgErr);

	void (*OnEvent)(long);
};