#pragma once
#include <vector>
#import "D:\\eloamCom\\bin\\eloamCom.dll" no_namespace


class CEloamGlobalCallback : public _IEloamGlobalEvents
{
public:
	CEloamGlobalCallback(){};
	~CEloamGlobalCallback(void){};
public:
	void setDevice(class CVYDevice *pDevice){m_pDevice=pDevice;};

	// Since we completely control callback object lifetime
	// there is no need to implement reference counter
	ULONG STDMETHODCALLTYPE AddRef()
	{
		return 1;
	}
	ULONG STDMETHODCALLTYPE Release()
	{
		return 1;
	}

	// IUnknown::QueryInterface implementation
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppObject)
	{
		*ppObject = NULL;
		if (riid == __uuidof(IUnknown)
			|| riid == __uuidof(_IEloamGlobalEvents))
		{
			*ppObject = this;
			AddRef();
			return S_OK;
		} 

		return E_NOINTERFACE;
	}


	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
		/* [out] */ __RPC__out UINT *pctinfo)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( 
		/* [in] */ UINT iTInfo,
		/* [in] */ LCID lcid,
		/* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo)	
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
		/* [in] */ __RPC__in REFIID riid,
		/* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
		/* [range][in] */ __RPC__in_range(0,16384) UINT cNames,
		/* [in] */ LCID lcid,
		/* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId)
	{
		return S_OK;
	}

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
		/* [in] */ DISPID dispIdMember,
		/* [in] */ REFIID riid,
		/* [in] */ LCID lcid,
		/* [in] */ WORD wFlags,
		/* [out][in] */ DISPPARAMS *pDispParams,
		/* [out] */ VARIANT *pVarResult,
		/* [out] */ EXCEPINFO *pExcepInfo,
		/* [out] */ UINT *puArgErr);

private:

	CVYDevice *m_pDevice;
};


/*

lpDlg 对话框句柄

ret = 1 指纹
ret = 2 身份证
ret = 3 摄像头到达
ret = 4 摄像头丢失

*/

typedef void (*CallBack)(DWORD lpDlg);
typedef void (*CallBackEx)(DWORD lpDlg, long index);

class CVYDevice
{
public:
	enum DEVICE{
		CAM1,
		CAM2,
		PRINT,
		IDCRAD
	};
public:
	CVYDevice(void);
	~CVYDevice(void);

private:
	DWORD m_cookie;
	CEloamGlobalCallback m_oCallback;

	bool showing;

	DEVICE id;
	IEloamViewPtr m_pView;
	IEloamVideoPtr m_pVideo1;
	IEloamVideoPtr m_pVideo2;

public:
	// cbk:回调函数
	// dlg:对话框句柄
	DWORD m_pDlgPrint;
	CallBack m_pCallbackPrint;
	void SetPrintCallBack(CallBack cbk,DWORD dlg);
	DWORD m_pDlgIDCard;
	CallBack m_pCallbackIDCard;
	void SetIDCardCallBack(CallBack cbk,DWORD dlg);
	DWORD m_pDlgArrive;
	CallBackEx m_pCallbackArrive;
	void SetArriveCallBack(CallBackEx cbk,DWORD dlg);
	DWORD m_pDlgLeave;
	CallBackEx m_pCallbackLeave;
	void SetLeaveCallBack(CallBackEx cbk,DWORD dlg);

	IEloamGlobalPtr m_pGlobal;
	IEloamDevicePtr Devices[3];
	std::vector<_bstr_t> Name;
	std::vector<IEloamDevicePtr> m_vDevice;

	int idx;
	int dbt;
private:
	IEloamVideoPtr GetVideo();
public:
	//通过此函数获取全局对象，可以直接调用问控件相关操作，防止接口遗漏
	IEloamGlobalPtr GetGlobal();

public:
	void Setup();

	void SetView(IEloamViewPtr pView);

	bool Open(DEVICE dev);
	bool Close(DEVICE dev);
	bool SaveImage(DEVICE dev,bstr_t path);

	bool OpenPrint();
	bool ClosePrint();

	bool OpenIDCard();
	bool CloseIDCard();
	// flag 含义见代码
	BSTR GetIdCardData(LONG flag);


	LONG GetResolutionCount();
	LONG GetResolutionWidth(LONG resIdx);
	LONG GetResolutionHeight(LONG resIdx);
	bool SetResolution(LONG idx);


	bool SelectCamera(DEVICE id=CAM1);

	bool OpenCamera(LONG resIdx=0);
	bool CloseCamera();

	bool IsShowing();

	bool ShowOn();
	bool ShowOff();

	bool Capture(bstr_t path);

	bool EnDeskew(bool isDeskew=true);
	bool EnSelect(bool isSelect=true);

};

