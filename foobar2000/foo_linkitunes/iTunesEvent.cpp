#include "iTunesEvent.h"

CITunesEventSink::CITunesEventSink()
{
	m_dwRefCount = 0;
	ITypeLib* pITypeLib = NULL;
	HRESULT     hr = ::LoadRegTypeLib(LIBID_iTunesLib,
		1, 5, // Major/Minor version numbers
		0x00,
		&pITypeLib);
	// Get type information for the interface of the object.
	hr = pITypeLib->GetTypeInfoOfGuid(DIID__IiTunesEvents, &m_pITypeInfo);
	pITypeLib->Release();
}
ULONG STDMETHODCALLTYPE CITunesEventSink::AddRef()
{
	InterlockedIncrement(&m_dwRefCount);
	// http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/jpdllpro/html/_win32_interlockedincrement.asp
	return m_dwRefCount;
}
ULONG STDMETHODCALLTYPE CITunesEventSink::Release()
{
	InterlockedDecrement(&m_dwRefCount);
	if (m_dwRefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_dwRefCount;
}
HRESULT STDMETHODCALLTYPE CITunesEventSink::QueryInterface(REFIID iid, void **ppvObject)
{
	if ((iid == IID_IDispatch) || (iid == DIID__IiTunesEvents))
	{
		m_dwRefCount++;
		*ppvObject = this;//(_IiTunesEvents *)this;
		return S_OK;
	}
	if (iid == IID_IUnknown)
	{
		m_dwRefCount++;
		*ppvObject = this;//(IUnknown *)this;
		return S_OK;
	}
	return E_NOINTERFACE;
}
HRESULT STDMETHODCALLTYPE CITunesEventSink::GetTypeInfoCount(UINT* pctinfo)
{
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CITunesEventSink::GetTypeInfo(UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
{
	return E_NOTIMPL;
}
HRESULT __stdcall CITunesEventSink::GetIDsOfNames(const IID& iid, OLECHAR** arrayNames, UINT countNames,
	LCID, // Localization はサポートしない。
	DISPID* arrayDispIDs)
{
	if (iid != IID_NULL) {
		return DISP_E_UNKNOWNINTERFACE;
	}
	HRESULT hr = m_pITypeInfo->GetIDsOfNames(arrayNames, countNames, arrayDispIDs);
	return hr;
}
HRESULT STDMETHODCALLTYPE CITunesEventSink::Invoke(DISPID dispidMember, REFIID riid, LCID lcid,
	WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult,
	EXCEPINFO* pexcepinfo, UINT* puArgErr)
{
	if (OnEvent != NULL)
	{
		//PostMessage(ccm->hParentWnd, ccm->WM_ITUNES, (WPARAM)dispidMember, 0);
		OnEvent(dispidMember);
	}
	return S_OK;
}