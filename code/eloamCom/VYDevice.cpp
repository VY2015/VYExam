#include "StdAfx.h"
#include "VYDevice.h"


/* [local] */ HRESULT STDMETHODCALLTYPE CEloamGlobalCallback::Invoke( 
	/* [in] */ DISPID dispIdMember,
	/* [in] */ REFIID riid,
	/* [in] */ LCID lcid,
	/* [in] */ WORD wFlags,
	/* [out][in] */ DISPPARAMS *pDispParams,
	/* [out] */ VARIANT *pVarResult,
	/* [out] */ EXCEPINFO *pExcepInfo,
	/* [out] */ UINT *puArgErr)
{
	switch (dispIdMember)
	{
	case 1:	// DevChange
		{
			int type = pDispParams->rgvarg[2].lVal;
			if (1 == type)
			{
				 m_pDevice->idx = pDispParams->rgvarg[1].lVal;
				 m_pDevice->dbt = pDispParams->rgvarg[0].lVal;

				if (1 == m_pDevice->dbt) // 设备到达
				{
					IEloamDevicePtr pDevice = m_pDevice->m_pGlobal->CreateDevice(1, m_pDevice->idx);
					m_pDevice->Devices[pDevice->GetEloamType()] = pDevice;
					m_pDevice->m_vDevice.push_back(pDevice);
					
					m_pDevice->Name.push_back(pDevice->GetFriendlyName());
					if (m_pDevice->m_pCallbackArrive)
					{
						m_pDevice->m_pCallbackArrive(m_pDevice->m_pDlgArrive, pDevice->GetEloamType());
					}
					
				}
				else if (2 == m_pDevice->dbt) // 设备丢失
				{
					IEloamDevicePtr pDevice = m_pDevice->m_vDevice[m_pDevice->idx];
					m_pDevice->m_vDevice.erase(m_pDevice->m_vDevice.begin() + m_pDevice->idx);

					m_pDevice->Devices[m_pDevice->idx]=NULL;
					long index = pDevice->GetEloamType();
					if (m_pDevice->m_pCallbackLeave)
					{
						m_pDevice->m_pCallbackLeave(m_pDevice->m_pDlgLeave, index);
					}
				}
			}
		}
	break;
	case 2://ID Card
		if ( pDispParams->rgvarg[0].lVal == 1 )
		{
			if (m_pDevice->m_pCallbackIDCard)
			{
				m_pDevice->m_pCallbackIDCard(m_pDevice->m_pDlgIDCard);
			}
		}
		break;

	case 7://FingerPrint
		if ( pDispParams->rgvarg[0].lVal == 8)
		{
			if (m_pDevice->m_pCallbackPrint)
			{
				m_pDevice->m_pCallbackPrint(m_pDevice->m_pDlgPrint);
			}
		}
		break;
	}
	return 0;
}


CVYDevice::CVYDevice(void): showing(false),id(CAM1), m_pVideo1(NULL), m_pVideo2(NULL)
{
	m_pCallbackIDCard  = m_pCallbackPrint = NULL;
	m_pCallbackArrive = m_pCallbackLeave = NULL;
	memset(Devices,NULL,sizeof(Devices));
}

CVYDevice::~CVYDevice(void)
{

	m_pGlobal->DeinitBiokey();
	m_pGlobal->DeinitIdCard();
	m_pGlobal->DeinitDevs();

	IConnectionPointContainerPtr pConnectionPointContainer = NULL;
	m_pGlobal->QueryInterface(
		IID_IConnectionPointContainer, (void**)&pConnectionPointContainer);
	if (NULL != pConnectionPointContainer)
	{
		IConnectionPointPtr pConnectionPoint = NULL;
		pConnectionPointContainer->FindConnectionPoint(
			__uuidof(_IEloamGlobalEvents), &pConnectionPoint);
		if (NULL != pConnectionPoint) 
		{
			pConnectionPoint->Unadvise(m_cookie);
		}
	}

}

void CVYDevice::Setup()
{
	m_oCallback.setDevice(this);
	m_pGlobal.CreateInstance(__uuidof(EloamGlobal));
	IConnectionPointContainerPtr pConnectionPointContainer = NULL;
	m_pGlobal->QueryInterface(
		IID_IConnectionPointContainer, (void**)&pConnectionPointContainer);
	if (NULL != pConnectionPointContainer)
	{
		IConnectionPointPtr pConnectionPoint = NULL;
		pConnectionPointContainer->FindConnectionPoint(
			__uuidof(_IEloamGlobalEvents), &pConnectionPoint);
		if (NULL != pConnectionPoint) 
		{
			pConnectionPoint->Advise(&m_oCallback, &m_cookie);
		}
	}

	m_pGlobal->InitDevs();
	m_pGlobal->InitBiokey();
	m_pGlobal->InitIdCard();
}

void CVYDevice::SetPrintCallBack(CallBack cbk,DWORD dlg)
{
	m_pCallbackPrint = cbk;
	m_pDlgPrint = dlg;
}

void CVYDevice::SetIDCardCallBack(CallBack cbk,DWORD dlg)
{
	m_pCallbackIDCard = cbk;
	m_pDlgIDCard = dlg;
}

void CVYDevice::SetArriveCallBack(CallBackEx cbk,DWORD dlg)
{
	m_pCallbackArrive = cbk;
	m_pDlgArrive = dlg;
}

void CVYDevice::SetLeaveCallBack(CallBackEx cbk,DWORD dlg)
{
	m_pCallbackLeave = cbk;
	m_pDlgLeave = dlg;
}

IEloamGlobalPtr CVYDevice::GetGlobal()
{
	return m_pGlobal;
}

void CVYDevice::SetView(IEloamViewPtr pView)
{
	m_pView = pView;
}

bool CVYDevice::Open(DEVICE id)
{
	switch(id)
	{
	case CAM1: SelectCamera(CAM1);return OpenCamera();
	case CAM2: SelectCamera(CAM2);return OpenCamera();		
	case PRINT: return OpenPrint();
	case IDCRAD: return OpenIDCard();
	}
	return false;
}

bool CVYDevice::Close(DEVICE id)
{
	switch(id)
	{
	case CAM1:
	case CAM2: return CloseCamera();
	case PRINT: return ClosePrint();
	case IDCRAD: return CloseIDCard();
	}
	return false;
}

bool CVYDevice::SaveImage(DEVICE id,bstr_t path)
{
	IEloamImagePtr pImage=NULL;
	switch(id)
	{
	case CAM1: 
		{
			if (m_pVideo1)
			{
				pImage = m_pVideo1->CreateImage(0, m_pView);
			}
			else
			{
				return false;
			}			
		}
		break;
	case CAM2: 
		{
			if (m_pVideo2)
			{
				pImage = m_pVideo2->CreateImage(0, m_pView);
			}
			else
			{
				return false;
			}			
		}
		break;
	case PRINT: pImage = m_pGlobal->GetBiokeyImg();break;
	case IDCRAD: pImage = m_pGlobal->GetIdCardImage(1);break;
	}
	if (pImage)
	{
		return pImage->Save(path,0);
	}
	else
		return false;
}

bool CVYDevice::OpenPrint()
{
	return m_pGlobal->GetBiokeyFeature();
}

bool CVYDevice::ClosePrint()
{
	return m_pGlobal->StopGetBiokeyFeature();
}

bool CVYDevice::OpenIDCard()
{	
	return m_pGlobal->DiscernIdCard();
}

BSTR CVYDevice::GetIdCardData(LONG flag)
{
	return m_pGlobal->GetIdCardData(flag);
}

bool CVYDevice::CloseIDCard()
{
	return m_pGlobal->StopIdCardDiscern();
}

bool CVYDevice::SelectCamera(DEVICE id)
{
	this->id = id;
	if (id==CAM2)
	{
		IEloamVideo *m_pVideo = m_pVideo2;
	}
	else
		return false;
	return true;
}


IEloamVideoPtr CVYDevice::GetVideo()
{
	if (id==CAM1)
		return m_pVideo1;
	else if(id==CAM2)
		return m_pVideo2;
}

LONG CVYDevice::GetResolutionCount()
{
	IEloamVideoPtr pVideo = GetVideo(); 
	if (pVideo)
	{
		IEloamDevicePtr pDevice =  pVideo->GetDevice();
		return pDevice->GetResolutionCount();
	}
	else
	{
		return 0;
	}
}

LONG CVYDevice::GetResolutionWidth(LONG idx)
{
	IEloamVideoPtr pVideo = GetVideo(); 
	if (pVideo)
	{
		IEloamDevicePtr pDevice =  pVideo->GetDevice();
		return pDevice->GetResolutionWidth(idx);
	}
	else
	{
		return 0;
	}
}

LONG CVYDevice::GetResolutionHeight(LONG idx)
{
	IEloamVideoPtr pVideo = GetVideo(); 
	if (pVideo)
	{
		IEloamDevicePtr pDevice =  pVideo->GetDevice();
		return pDevice->GetResolutionHeight(idx);
	}
	else
	{
		return 0;
	}
}

bool CVYDevice::SetResolution(LONG idx)
{
	return OpenCamera(idx);
}

bool CVYDevice::OpenCamera(LONG resIdx)
{
	IEloamDevicePtr pDevice = Devices[id+1];
	switch(id)
	{
	case CAM1:
		if (pDevice)
		{
			m_pVideo1 = pDevice->CreateVideo(resIdx,1);
			return true;
		}
		break;
	case CAM2:
		if (pDevice)
		{
			m_pVideo2 = pDevice->CreateVideo(resIdx,1);
			return true;
		}
		break;
	}
	return false;
}

bool CVYDevice::IsShowing()
{
	return showing;
}

bool CVYDevice::CloseCamera()
{
	ShowOff();
	switch(id)
	{
	case CAM1: 
		if (m_pVideo1)
		{
			m_pVideo1.Release();
		}
		m_pVideo1 = NULL;
		return true;
	case CAM2:
		if (m_pVideo2)
		{
			m_pVideo2.Release();
		}
		m_pVideo2 = NULL;
		return true;
	}
	return false;
}

bool CVYDevice::ShowOn()
{
	if (m_pView)
	{
		IEloamVideoPtr pVideo = GetVideo(); 
		if (pVideo)
		{
			showing = true;
			m_pView->SetText("正在打开，请稍等。。。", RGB(255,255,255));
			return m_pView->SelectVideo(pVideo);
		}
	}
	return false;
}

bool CVYDevice::ShowOff()
{
	if (m_pView)
	{
		showing = false;
		m_pView->SetText("设备已关闭！",RGB(255,255,255));
		return m_pView->SelectNull();
	}
	else
		return false;
}

bool CVYDevice::Capture(bstr_t path)
{
	if (m_pView==NULL)
		return false;

	IEloamVideoPtr pVideo = GetVideo(); 
	IEloamImagePtr pImage = pVideo->CreateImage(0, m_pView);
	if (pImage==NULL)
		return false;
	if (pImage->Save(path, 0))
	{
		return m_pView->PlayCaptureEffect();
	}
	else
		return false;
}

bool CVYDevice::EnDeskew(bool isDeskew)
{
	IEloamVideoPtr pVideo = GetVideo(); 
	if (isDeskew)
		return pVideo->EnableDeskew(0);
	else
		return pVideo->DisableDeskew();
}

bool CVYDevice::EnSelect(bool isSelect)
{
	if (isSelect)
		return m_pView->SetState(2);
	else
		return m_pView->SetState(1);
}