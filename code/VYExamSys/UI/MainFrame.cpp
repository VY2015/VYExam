//commandMap
#include "stdafx.h"
#include "MainFrame.h"
#include "UpLoadProgressBar.h"
#include "id2GetPapers.h"
#include "id2GetPapers2.h"

int ComboIndex = 0;




CMainFrame::CMainFrame(void)
	: m_pView1(NULL)
	, m_pView2(NULL)
{
	//char buf1[2048];char buf2[2048];
	//memset(buf1,0,sizeof(buf1));
	//m_vyfeature1._pFeatureData=buf1;
	//m_vyfeature1._nFeatureSize=0;
	//memset(buf2,0,sizeof(buf2));
	//m_vyfeature2._pFeatureData=buf2;
	//m_vyfeature2._nFeatureSize=0;
	//if(!m_algo.Init(true))
	//	printf("Init error\n");
	m_key = string("justcoolveryescn");
	memset(m_LicencePath,'\0',sizeof(m_LicencePath));
	memset(m_LicenceHeaderPath,'\0',sizeof(m_LicenceHeaderPath));
	memset(m_curPicPath,'\0',sizeof(m_curPicPath));
	memset(m_hisPicPath,'\0',sizeof(m_hisPicPath));
	memset(m_lastPicPath,'\0',sizeof(m_lastPicPath));
	m_iRetry = 0;
	clearRoster();
	clearEntrance();
}

LPCTSTR CMainFrame::GetWindowClassName() const
{ 
	return _T("UIMainFrame"); 
}

UINT CMainFrame::GetClassStyle() const
{
	return UI_CLASSSTYLE_FRAME; 
}

void CMainFrame::OnFinalMessage(HWND /*hWnd*/)
{ 
	m_pm.RemovePreMessageFilter(this);
	delete this; 
}

void CMainFrame::Init()
{

}
void CMainFrame::clearRoster()
{
	memset(m_roster.Exam_id,'\0',sizeof(m_roster.Exam_id));
	memset(m_roster.Fix_register_id,'\0',sizeof(m_roster.Fix_register_id));
	m_roster.Fix_licence_type = 0;
	memset(m_roster.Fix_chs_surname,'\0',sizeof(m_roster.Fix_chs_surname));
	memset(m_roster.Fix_chs_name,'\0',sizeof(m_roster.Fix_chs_name));
	memset(m_roster.Fix_en_surname,'\0',sizeof(m_roster.Fix_en_surname));
	memset(m_roster.Fix_en_name,'\0',sizeof(m_roster.Fix_en_name));
	memset(m_roster.Fix_nation,'\0',sizeof(m_roster.Fix_nation));
	memset(m_roster.Fix_sex,'\0',sizeof(m_roster.Fix_sex));
	memset(m_roster.Fix_born,'\0',sizeof(m_roster.Fix_born));
	memset(m_roster.Fix_addr,'\0',sizeof(m_roster.Fix_addr));
	memset(m_roster.Fix_card_num,'\0',sizeof(m_roster.Fix_card_num));
	memset(m_roster.Fix_police,'\0',sizeof(m_roster.Fix_police));
	memset(m_roster.Fix_active,'\0',sizeof(m_roster.Fix_active));
	memset(m_roster.Full_name,'\0',sizeof(m_roster.Full_name));
	memset(m_roster.Nation,'\0',sizeof(m_roster.Nation));
	memset(m_roster.Sex,'\0',sizeof(m_roster.Sex));
	memset(m_roster.Born,'\0',sizeof(m_roster.Born));
	memset(m_roster.Addr,'\0',sizeof(m_roster.Addr));
	memset(m_roster.Card_num,'\0',sizeof(m_roster.Card_num));
	memset(m_roster.Police,'\0',sizeof(m_roster.Police));
	memset(m_roster.Active,'\0',sizeof(m_roster.Active));
	m_roster.Attend=0;         //是否缺席
	memset(m_roster.Seat,'\0',sizeof(m_roster.Seat));
	memset(m_roster.Licence_photo_path,'\0',sizeof(m_roster.Licence_photo_path));
	memset(m_roster.Head_photo_path,'\0',sizeof(m_roster.Head_photo_path));
	memset(m_roster.Fix_history_photo_path,'\0',sizeof(m_roster.Fix_history_photo_path));
	m_roster.Times=0;
}
void CMainFrame::clearEntrance()
{
	char	Exam_id[50];
	memset(m_entrance.Exam_id,'\0',sizeof(m_entrance.Exam_id));
	char	Fix_register_id[50];
	memset(m_entrance.Fix_register_id,'\0',sizeof(m_entrance.Fix_register_id));
	char	Entrance_time[50];
	memset(m_entrance.Entrance_time,'\0',sizeof(m_entrance.Entrance_time));
	char	Entrance_photo_path[150];
	memset(m_entrance.Entrance_photo_path,'\0',sizeof(m_entrance.Entrance_photo_path));
	m_entrance.Ghost_check=0;
	m_entrance.Machine_check=0;
	m_entrance.Man_check=0;
	m_entrance.History_check=0;
	m_entrance.Times=0;
}
void CStringFormat(CString &str,char *format)
{
	const size_t s_size = (str.GetLength()+1)*2;
	char * pstr = new char[s_size];
	size_t sz=0;
	wcstombs_s(&sz,pstr,s_size,str,_TRUNCATE);
	int a = atoi((const char*)pstr);

	if(!strcmp("%02d",format))
		str.Format(L"%02d",a);
	else if(!strcmp("%04d",format))
		str.Format(L"%04d",a);
}
CString getPathFromFullPath(const CString& szPath)
{
	CString str;
	int index = szPath.ReverseFind((wchar_t)'\/');
	if (index == -1)
	{
		index = szPath.ReverseFind((wchar_t)"\\\\");
	}
	if (index == -1)
	{
		return CString();
	}
	str = szPath.Left(index);
	return str;
}
bool CreateMultipleDirectory(const CString& szPath)
{
	CString strDir(szPath);//存放要创建的目录字符串
	//确保以'\'结尾以创建最后一个目录
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\/'))
	{
		strDir.AppendChar(_T('\/'));
	}
	std::vector<CString> vPath;//存放每一层目录字符串
	CString strTemp;//一个临时变量,存放目录字符串
	bool bSuccess = false;//成功标志
	//遍历要创建的字符串
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\/')) 
		{//如果当前字符不是'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else if(strDir.GetAt(i)==_T('.'))
		{
			;//对路径中存在的.或..直接读取下一个
		}
		else
		{//如果当前字符是'\\'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(_T('\/'));
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}

	return bSuccess;
}
//将wstring转换成string  
string wstring2string(wstring wstr)  
{  
	string result;  
	//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);  
	char* buffer = new char[len + 1];  
	//宽字节编码转换成多字节编码  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);  
	buffer[len] = '\0';  
	//删除缓冲区并返回值  
	result.append(buffer);  
	delete[] buffer;  
	return result;  
}  

void CMainFrame::showIDCardUI()
{
	CTextUI *pText = NULL;
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1Name_text")));
	pText->SetText(CString(m_roster.Full_name));
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1Sex_text")));
	if (CStringA(m_roster.Sex) == CStringA("1"))
	{
		pText->SetText((CString)"男");
	}
	else
		pText->SetText((CString)"女");
	
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1Race_text")));
	pText->SetText((CString)m_roster.Nation);
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1birthDay_text")));
	pText->SetText((CString)m_roster.Born);
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1Address_text")));
	pText->SetText((CString)m_roster.Addr);
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1passNum_text")));
	pText->SetText((CString)m_roster.Card_num);
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1passValidity")));
	pText->SetText((CString)m_roster.Active);
	CButtonUI *pButton = static_cast<CButtonUI*>(m_pm.FindControl(_T("id1passPhoto_btn")));
	pButton->SetBkImage((CString)m_LicenceHeaderPath);	
	//花名册从库中取根据身份证
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1registerNum_text")));
	pText->SetText(CString("2"));//?
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1cnsName_text")));
	pText->SetText((CString)m_roster.Fix_chs_surname);
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1ensName_text")));
	pText->SetText((CString)m_roster.Fix_en_surname);
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1passType_text")));
	CString str;
	str.Format(_T("%d"),m_roster.Fix_licence_type);
	pText->SetText((LPCTSTR)str);
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1examSource_text")));
	pText->SetText((CString)"报名系统");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1cnName_text")));
	pText->SetText((CString)m_roster.Fix_chs_name);
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1enName_text")));
	pText->SetText((CString)m_roster.Fix_en_name);
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1nextPassNum_text")));
	pText->SetText((CString)m_roster.Fix_card_num);
	//考场信息
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1inNumber_text")));
	str.Format(_T("%d"),m_nTimes);
	pText->SetText((LPCTSTR)str);
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1examCode_text")));
	pText->SetText((CString)g_login.examRoom);
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1examItem_text")));
	pText->SetText((CString)g_login.examItem);
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1examData_text")));
	pText->SetText((CString)g_examTime);
	//历史照(EXAM_ROSTER)，上一张(EXAM_ENTRANCE)，当前照(放到对比)??
	CButtonUI *pHistoryPic = static_cast<CButtonUI*>(m_pm.FindControl(_T("id1hisbtn")));
	//解析bin为png图片
	CString strPathOut;
	CStringA strTmp;
	strTmp.Format("%s.png",m_regID.data());
	strPathOut = CString("./local/20150917/Test/Report/History/") + CString(strTmp);
	m_cEncryptyFile.AesEncryptorDecryptFile((string)(m_roster.Fix_history_photo_path), m_key, (string)wstring2string(strPathOut.GetBuffer()),false);
	strcpy_s(m_hisPicPath,sizeof(m_hisPicPath),CStringA(strPathOut));
	pHistoryPic->SetBkImage(CString(strPathOut));
	CButtonUI *pLastPic = static_cast<CButtonUI*>(m_pm.FindControl(_T("id1prebtn")));
	//上一张从entrance表里取?
	vector<EXAM_ENTRANCE> v_examEntrances;
	g_db.QueryEntranceOfPerson(m_examid,m_regID,v_examEntrances);
	
	for (vector<EXAM_ENTRANCE>::iterator it=v_examEntrances.begin();it!=v_examEntrances.end();it++)
	{
		if (it->Times == m_nTimes-1)//当前入场次数-1即为上一次入场
		{
			strcpy_s(m_entrance.Entrance_photo_path,sizeof(m_entrance.Entrance_photo_path),it->Entrance_photo_path);
		}
	}
	if (m_nTimes == 1) //没有上一张照片
	{
	}
	else
	{
		strTmp.Format("%s_%d.png",m_regID.data(),m_nTimes-1);
		strPathOut = CString("./local/20150917/Test/Report/Pic/Entrance/") + CString(strTmp);
		m_cEncryptyFile.AesEncryptorDecryptFile((string)(m_entrance.Entrance_photo_path), m_key, (string)wstring2string(strPathOut.GetBuffer()),false);
		strcpy_s(m_lastPicPath,sizeof(m_lastPicPath),CStringA(strPathOut));
		pLastPic->SetBkImage(CString(strPathOut));
	}
}

void IDCardCheck(DWORD dlg)//二代身份证验证回调
{
	CMainFrame *pFrame = (CMainFrame*)dlg;
	CString idNo = g_oVYDevice.GetIdCardData(8);
	//检查花名册是否存在此人
	//1.数据库根据考场号和报名号获取入场次数
	std::string exam_id,regID;
	//g_db.GetRosterPersonNum(exam_id);
	exam_id = string(g_examId);
	regID = "2";
	pFrame->m_examid = exam_id;
	g_db.GetRosterPersonRegID(exam_id,string(wstring2string(idNo.GetBuffer())),regID);
	pFrame->m_regID = regID;
	g_db.GetRosterPersonInfo(exam_id,regID,pFrame->m_roster);//
	int iEntryTimesMax=pFrame->m_roster.Times;	
	pFrame->m_nTimes = iEntryTimesMax + 1;//当前入场次数+1在比对通过后插入数据库，不通过不进行数据库操作
	//2.缓存身份信息
	strcpy_s(pFrame->m_roster.Full_name,sizeof(pFrame->m_roster.Full_name),CStringA(g_oVYDevice.GetIdCardData(1)));
	CString seX = CString("-1");
	if (CStringA(g_oVYDevice.GetIdCardData(2)) == CStringA("男"))
	{
		seX = "1";
	}
	else
		seX = "0";
	
	strcpy_s(pFrame->m_roster.Sex, sizeof(pFrame->m_roster.Sex),CStringA(seX));//8
	strcpy_s(pFrame->m_roster.Nation, sizeof(pFrame->m_roster.Nation), CStringA(g_oVYDevice.GetIdCardData(3)));//32
	CString idBorn = g_oVYDevice.GetIdCardData(4);
	idBorn += CString("年");
	CString mon0 = g_oVYDevice.GetIdCardData(5);
	CStringFormat(mon0,"%02d");
	idBorn += mon0;
	idBorn += CString("月");
	CString day0 = g_oVYDevice.GetIdCardData(6);
	CStringFormat(day0,"%02d");
	idBorn += day0;
	idBorn += CString("日");
	strcpy_s(pFrame->m_roster.Born, sizeof(pFrame->m_roster.Born), CStringA(idBorn));
	strcpy_s(pFrame->m_roster.Addr, sizeof(pFrame->m_roster.Addr), CStringA(g_oVYDevice.GetIdCardData(7)));
	strcpy_s(pFrame->m_roster.Card_num, sizeof(pFrame->m_roster.Card_num), CStringA(g_oVYDevice.GetIdCardData(8)));
	strcpy_s(pFrame->m_roster.Police, sizeof(pFrame->m_roster.Police), CStringA(g_oVYDevice.GetIdCardData(9)));
	CString idActive = g_oVYDevice.GetIdCardData(10);
	idActive += CString(".");
	CString mon = g_oVYDevice.GetIdCardData(11);
	CStringFormat(mon,"%02d");
	idActive += mon;
	idActive += CString(".");
	CString day = g_oVYDevice.GetIdCardData(12);
	CStringFormat(day,"%02d");
	idActive += day;
	idActive += CString("-");
	idActive += g_oVYDevice.GetIdCardData(13);
	idActive += CString(".");
	CString mon2 = g_oVYDevice.GetIdCardData(14);
	CStringFormat(mon2,"%02d");
	idActive += mon2;
	idActive += CString(".");
	CString day2 = g_oVYDevice.GetIdCardData(15);
	CStringFormat(day2,"%02d");
	idActive += day2;
	//strcpy_s(pFrame->m_roster.Active, sizeof(pFrame->m_roster.Active), CStringA(idActive));//10.11.12-13.14.15
	//校验身份证有效期跟考试时间
	bool bPaperActive = false;//
	CString csTime = idActive.Mid(11);//2017.09.10
	csTime.Replace(L".", L"");
	CTime cTime(_wtoi(csTime.Mid(0, 4)), _wtoi(csTime.Mid(4, 2)), _wtoi(csTime.Mid(6, 2)), 0, 0, 0);//+15天判断有效性
	cTime += CTimeSpan(14,0,0,0);
	csTime.Format(L"%04d%02d%02d", cTime.GetYear(), cTime.GetMonth(), cTime.GetDay());
	CString csExamTime("20150917");//为空有效,服务器时间==考试时间??
	if (csTime.Compare(csExamTime) >= 0)
	{
		bPaperActive = true;//有效
		strcpy_s(pFrame->m_roster.Active,sizeof(pFrame->m_roster.Active),CStringA("有效"));
	}
	else
	{
		bPaperActive = false;//无效
		strcpy_s(pFrame->m_roster.Active,sizeof(pFrame->m_roster.Active),CStringA("无效"));

	}
	//3.缓存证件照片文件
	IEloamGlobalPtr globalPtr = g_oVYDevice.GetGlobal();
	IEloamImagePtr  licencePic = globalPtr->GetIdCardImage(4);
	if (licencePic)
	{	
		CString strPath = CString("./local/20150917/Test/Report/Pic/Licence/");

		if (!PathFileExists(strPath))
		{
			CreateMultipleDirectory(strPath);
		}
		CString strPathSrc;
		strPathSrc = strPath +CString("licence.png");
		licencePic->Save((_bstr_t)strPathSrc,0);
		//图片加密后的路径存到花名册,
		//CString strPathOut;
		//CStringA strTmp;
		//strTmp.Format("%s_%d.bin",regID.data(),pFrame->m_nTimes);
		//strPathOut = strPath + CString(strTmp);
		//pFrame->m_cEncryptyFile.AesEncryptorDecryptFile((string)wstring2string(strPathSrc.GetBuffer()), pFrame->m_key, (string)wstring2string(strPathOut.GetBuffer()),true);
		strcpy_s(pFrame->m_LicencePath,sizeof(pFrame->m_LicencePath),CStringA(strPathSrc));
	    //strcpy_s(pFrame->m_roster.Licence_photo_path, sizeof(pFrame->m_roster.Licence_photo_path), CStringA(strPathOut));
	}
	//4.缓存人脸大头照文件
	IEloamImagePtr facePic = globalPtr->GetIdCardImage(1);
	if (facePic)
	{
		CString strPath = CString("./local/20150917/Test/Report/Pic/LicenceHeader/");
		
		if (!PathFileExists(strPath))
		{
			CreateMultipleDirectory(strPath);
		}
		CString strPathSrc;
		strPathSrc = strPath + CString("licenceHeader.png");
		facePic->Save((_bstr_t)strPathSrc,0);
		//CString strPathOut;
		//CStringA strTmp;
		//strTmp.Format("%s_%d.bin",regID.data(),pFrame->m_nTimes);
		//strPathOut = strPath + CString(strTmp);
		//pFrame->m_cEncryptyFile.AesEncryptorDecryptFile((string)wstring2string(strPathSrc.GetBuffer()), pFrame->m_key, (string)wstring2string(strPathOut.GetBuffer()),true);
		strcpy_s(pFrame->m_LicenceHeaderPath,sizeof(pFrame->m_LicenceHeaderPath),CStringA(strPathSrc));
		//strcpy_s(pFrame->m_roster.Head_photo_path, sizeof(pFrame->m_roster.Head_photo_path), CStringA(strPathOut));
	}
	//5.缓存信息界面显示
	pFrame->showIDCardUI();
	//...缓存当前照在比对时进行
}

void CMainFrame::initID2Check()
{
	//1.初始化考场信息
	CTextUI* m_pExamCode = static_cast<CTextUI*>(m_pm.FindControl(_T("id2examCode_text")));
	m_pExamCode->SetText(CString(g_login.examRoom));
	CTextUI* m_pExamItem = static_cast<CTextUI*>(m_pm.FindControl(_T("id2examItem_text")));
	m_pExamItem->SetText(CString(g_login.examItem));
	CTextUI* m_pExamDate = static_cast<CTextUI*>(m_pm.FindControl(_T("id2examData_text")));
	m_pExamDate->SetText(CString(g_examTime));
	
	//2.初始化选择考生下拉框
	vector<string> vRegIds;
	g_db.GetRosterPersons(string(g_examId),vRegIds);
	CComboUI* m_pCbtn_add = static_cast<CComboUI*>(m_pm.FindControl(_T("choiceperson_combo")));
	for (int i=0;i<vRegIds.size();i++)
	{
		EXAM_ROSTER examRoster;
		string regID = vRegIds.at(i);
		g_db.GetRosterPersonInfo(string(g_examId),regID,examRoster);
		char cName[50];
		memset(cName,'\0',sizeof(cName));
		strcpy(cName,examRoster.Fix_chs_surname);
		strcat(cName,examRoster.Fix_chs_name);
		CString path = CString(regID.c_str())+CString("|")+CString(cName)+CString("|")+CString(examRoster.Fix_card_num);
		CListLabelElementUI *pLabel = new CListLabelElementUI;        
		pLabel->SetAttribute(_T("text"), path);      
		m_pCbtn_add->Add(pLabel);
	}
}

void CMainFrame::OnPrepare()
{
	ShowReport();
	InitMenu();

	InitVideo();
	InitList();
	g_oVYDevice.SetIDCardCallBack(IDCardCheck, (DWORD)this);
	g_oVYDevice.Open(CVYDevice::IDCRAD);

	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("ContainerUI1")));
	//pControl->SetBkImage(L"button_norm_green.jpg");

	CButtonUI* pBtn;        
	pBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("prebtn")));
	pBtn->SetBkImage(L"button_norm_red.jpg");
}
#include "DlgChoPerson.h"
void CMainFrame::capCurrentPicCompare()
{
	//1.缓存当前照片			
	CString strPath = CString("./local/20150917/Test/Report/Pic/Entrance/");
	if (!PathFileExists(strPath))
	{
		CreateMultipleDirectory(strPath);
	}
	CStringA strTmp;
	strTmp.Format("entrance%d.png",m_iRetry++);
	CString strPath1 = strPath+CString(strTmp);/*CString("entrance.png")*/
	g_oVYDevice.SetView(m_pView1);
	g_oVYDevice.SelectCamera(CVYDevice::CAM2);
	if (g_oVYDevice.Capture((_bstr_t)strPath1))
	{
		//显示到当前照,缓存png文件无论是否比对成功删除
		CButtonUI *pCurPic = static_cast<CButtonUI*>(m_pm.FindControl(_T("id1curbtn")));
		pCurPic->SetBkImage(strPath1);
		strcpy_s(m_curPicPath,sizeof(m_curPicPath),CStringA(strPath1));
		//比对成功加密，存到入场表缓存区		
	}
	else
	{
		MessageBoxA(NULL,"请重新采集当前照片","当前照片采集失败",0);
	}
	//2.大头照与当前照比对
	CString strPath2 = CString(m_LicenceHeaderPath);
	if (!PathFileExists(strPath1)||!PathFileExists(strPath2))
	{
		//异常
		MessageBoxA(NULL,"请重刷身份证或检查身份证","当前照片采集失败",0);
		doClearUIandBuffer();
		return;
	}
	//m_algo.Extract(string(CStringA(strPath1)),m_vyfeature1);
	//m_algo.Extract(string(CStringA(strPath2)),m_vyfeature2);
	//double ret;
	//ret=m_algo.Compare(m_vyfeature1,m_vyfeature2,0);
	//if (ret<0)
	//{
	//	//抛出异常;
	//	MessageBoxA(NULL,LPCSTR("证件大头像和当前照片不匹配"),LPCSTR("提示"),0);
	//}	
	//3.
	if (1)//两张图片相同,通过
	{
		//下一个，人工标记?
		DialogPassed passedDlg(strPath2,strPath1,this);
		UINT ret = passedDlg.DoModal();
		if (ret == IDOK)//下一个
		{
			doIDPassed();
			//MessageBoxA(NULL,"ok","ok",0);
		}
		else if (ret == IDCANCEL)//人工标记
		{
			doIDPassed();
			//MessageBoxA(NULL,"cancel","cancel",0);
		}
		else if(ret == IDC_REFUSE2)//拒绝在对话框内部做
		{
		}		
	}
	else
	{
		IDDialogNotPassed idDlg(strPath2,strPath1,this);
		UINT ret = idDlg.DoModal();
		if (ret == IDOK)//重新核验
		{
			CButtonUI *pCurPic = static_cast<CButtonUI*>(m_pm.FindControl(_T("id1curbtn")));
			pCurPic->SetBkImage(CString(""));	
			DeleteFile(CString(m_curPicPath));
			return;
		}
		else if(ret == IDCANCEL)//强制通过
		{
			doIDPassed();
		}
		else//拒绝入场
		{//执行第4步
		}
	}
	//4.无论通过与否清空缓存
	doClearUIandBuffer();
}
void CMainFrame::doIDPassed()
{
	//通过后加密证件照
	CString strPath = getPathFromFullPath(CString(m_LicencePath));
	CString strPathOut;
	CStringA strTmp;
	strTmp.Format("/%s.bin",m_regID.data());
	strPathOut = strPath + CString(strTmp);
	m_cEncryptyFile.AesEncryptorDecryptFile((string)(m_LicencePath), m_key, (string)wstring2string(strPathOut.GetBuffer()),true);
	strcpy_s(m_roster.Licence_photo_path, sizeof(m_roster.Licence_photo_path), CStringA(strPathOut));
	//大头照
	strPath = getPathFromFullPath(CString(m_LicenceHeaderPath));
	strPathOut = strPath + CString(strTmp);
	m_cEncryptyFile.AesEncryptorDecryptFile((string)m_LicenceHeaderPath, m_key, (string)wstring2string(strPathOut.GetBuffer()),true);
	strcpy_s(m_roster.Head_photo_path, sizeof(m_roster.Head_photo_path), CStringA(strPathOut));

	//通过后才对当前照片进行加密缓存
	strPath = getPathFromFullPath(CString(m_curPicPath));
	strTmp.Format("/%s_%d.bin",m_regID.data(),m_nTimes);
	strPathOut = strPath + CString(strTmp);
	m_cEncryptyFile.AesEncryptorDecryptFile((string)(m_curPicPath), m_key, (string)wstring2string(strPathOut.GetBuffer()),true);
	strcpy_s(m_entrance.Entrance_photo_path,sizeof(m_entrance.Entrance_photo_path),CStringA(strPathOut));

	//6.图片路径，人员信息，入数据库
	g_db.UpdateRosterPersonInfo(m_examid,m_regID,m_roster);	//花名册修改字段
	////EXAM_ENTRANCE的entrance_photo_path当前照?
	g_db.UpdateEntrance(m_examid,m_regID,m_entrance,m_nTimes);
}
void CMainFrame::doClearBuffer()
{
	m_regID = "";
	m_iRetry = 0;
	m_nTimes = 0;
	m_examid = "";
	DeleteFile((CString)m_hisPicPath);
	DeleteFile((CString)m_curPicPath);
	DeleteFile((CString)m_LicenceHeaderPath);
	DeleteFile((CString)m_LicencePath);
	DeleteFile((CString)m_lastPicPath);
	memset(m_hisPicPath,'\0',sizeof(m_hisPicPath));
	memset(m_curPicPath,'\0',sizeof(m_curPicPath));
	memset(m_LicenceHeaderPath,'\0',sizeof(m_LicenceHeaderPath));
	memset(m_LicencePath,'\0',sizeof(m_LicencePath));
	memset(m_lastPicPath,'\0',sizeof(m_lastPicPath));
	clearRoster();
	clearEntrance();
}

void CMainFrame::doClearUIandBuffer()
{
	CTextUI *pText = NULL;
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1Name_text")));
	pText->SetText(CString(""));
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1Sex_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1Race_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1birthDay_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1Address_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1passNum_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1passValidity")));
	pText->SetText((CString)"");
	CButtonUI *pButton = static_cast<CButtonUI*>(m_pm.FindControl(_T("id1passPhoto_btn")));
	pButton->SetBkImage((CString)"");	
	//花名册从库中取根据身份证
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1registerNum_text")));
	pText->SetText(CString(""));//?
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1cnsName_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1ensName_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1passType_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1examSource_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1cnName_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1enName_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1nextPassNum_text")));
	pText->SetText((CString)"");
	//考场信息
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1inNumber_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1examCode_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1examItem_text")));
	pText->SetText((CString)"");
	pText = static_cast<CTextUI*>(m_pm.FindControl(_T("id1examData_text")));
	pText->SetText((CString)"");
	CButtonUI *pHistoryPic = static_cast<CButtonUI*>(m_pm.FindControl(_T("id1hisbtn")));
	pHistoryPic->SetBkImage(CString(""));
	CButtonUI *pLastPic = static_cast<CButtonUI*>(m_pm.FindControl(_T("id1prebtn")));
	pLastPic->SetBkImage(CString(""));
	CButtonUI *pCurPic = static_cast<CButtonUI*>(m_pm.FindControl(_T("id1curbtn")));
	pCurPic->SetBkImage(CString(""));	
	//7.清空缓存.
	//对比通过/不通过都要清空各个目录下的png图片,不通过还有删除bin
	doClearBuffer();
}
void CMainFrame::doClearID2UIandBuffer()
{
	CTextUI * pTextUI;
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2registerNum_text")));
	pTextUI->SetText((CString)"");
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2cnsName_text")));
	pTextUI->SetText((CString)"");
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2ensName_text")));
	pTextUI->SetText((CString)"");
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2passType_text")));
	pTextUI->SetText(/*eRoster.Fix_licence_type*/(CString)"");
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2examSource_text")));
	pTextUI->SetText(/*eRoster.*/(CString)"");
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2cnName_text")));
	pTextUI->SetText((CString)"");
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2enName_text")));
	pTextUI->SetText((CString)"");
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2passNum_text")));
	pTextUI->SetText((CString)"");
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2inNumber_text")));
	pTextUI->SetText(CString(""));
	CButtonUI *pHistoryPic = static_cast<CButtonUI*>(m_pm.FindControl(_T("id2hisbtn")));
	pHistoryPic->SetBkImage(CString(""));
	CButtonUI *pLastPic = static_cast<CButtonUI*>(m_pm.FindControl(_T("id2prebtn")));
	pLastPic->SetBkImage(CString(""));
	CButtonUI *pCurPic = static_cast<CButtonUI*>(m_pm.FindControl(_T("id2curbtn")));
	pCurPic->SetBkImage(CString(""));
	doClearBuffer();
}

void CMainFrame::showID2UI(EXAM_ROSTER eRoster)
{	
	CTextUI * pTextUI;
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2registerNum_text")));
	pTextUI->SetText((CString)eRoster.Fix_register_id);
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2cnsName_text")));
	pTextUI->SetText((CString)eRoster.Fix_chs_surname);
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2ensName_text")));
	pTextUI->SetText((CString)eRoster.Fix_en_surname);
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2passType_text")));
	pTextUI->SetText(/*eRoster.Fix_licence_type*/(CString)"其他来源");
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2examSource_text")));
	pTextUI->SetText(/*eRoster.*/(CString)"报名系统");
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2cnName_text")));
	pTextUI->SetText((CString)eRoster.Fix_chs_name);
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2enName_text")));
	pTextUI->SetText((CString)eRoster.Fix_en_name);
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2passNum_text")));
	pTextUI->SetText((CString)eRoster.Fix_card_num);
	pTextUI = static_cast<CTextUI*>(m_pm.FindControl(_T("id2inNumber_text")));
	CString times;
	times.Format(_T("%d"),eRoster.Times+1);
	pTextUI->SetText(times);
	//历史照，上一张
	CButtonUI *pHistoryPic = static_cast<CButtonUI*>(m_pm.FindControl(_T("id2hisbtn")));
	//解析bin为png图片
	CString strPathOut;
	CStringA strTmp;
	strTmp.Format("%s.png",eRoster.Fix_register_id);
	strPathOut = CString("./local/20150917/Test/Report/History/") + CString(strTmp);
	m_cEncryptyFile.AesEncryptorDecryptFile((string)(eRoster.Fix_history_photo_path), m_key, (string)wstring2string(strPathOut.GetBuffer()),false);
	strcpy_s(m_hisPicPath,sizeof(m_hisPicPath),CStringA(strPathOut));
	pHistoryPic->SetBkImage(CString(strPathOut));
	CButtonUI *pLastPic = static_cast<CButtonUI*>(m_pm.FindControl(_T("id1prebtn")));
	//上一张从entrance表里取?
	vector<EXAM_ENTRANCE> v_examEntrances;
	g_db.QueryEntranceOfPerson(g_examId,eRoster.Fix_register_id,v_examEntrances);

	for (vector<EXAM_ENTRANCE>::iterator it=v_examEntrances.begin();it!=v_examEntrances.end();it++)
	{
		if (it->Times == eRoster.Times)//当前入场次数-1即为上一次入场
		{
			strcpy_s(m_entrance.Entrance_photo_path,sizeof(m_entrance.Entrance_photo_path),it->Entrance_photo_path);
		}
	}
	if (eRoster.Times == 0) //没有上一张照片
	{
	}
	else
	{
		strTmp.Format("%s_%d.png",eRoster.Fix_register_id,eRoster.Times);
		strPathOut = CString("./local/20150917/Test/Report/Pic/Entrance/") + CString(strTmp);
		m_cEncryptyFile.AesEncryptorDecryptFile((string)(m_entrance.Entrance_photo_path), m_key, (string)wstring2string(strPathOut.GetBuffer()),false);
		strcpy_s(m_lastPicPath,sizeof(m_lastPicPath),CStringA(strPathOut));
		pLastPic->SetBkImage(CString(strPathOut));
	}

}

void CMainFrame::Notify(TNotifyUI& msg)
{
	TRACE(L"%s\n\n",msg.sType);
	if( msg.sType == _T("windowinit") )
		OnPrepare();
	if( msg.sType == _T("dbclick")) 
	{   
			if(msg.pSender->GetName() == _T("id2compare"))
			{
				AfxMessageBox(L"doubledounble"); return;
			}
	}
	else if( msg.sType == _T("click") ) 
	{
		if(msg.pSender->GetName() == _T("id1compare"))
		{ 
			capCurrentPicCompare();
			return;				
		}
		//@feng 非二代验证弹出窗口
		if(msg.pSender->GetName() == _T("id2GetPapers"))
		{ 
			Cid2GetPapersWnd* pId2GetPapersFrame = new Cid2GetPapersWnd;
			if(pId2GetPapersFrame == NULL) return;
			pId2GetPapersFrame->Create(this->GetHWND(), NULL, UI_WNDSTYLE_CHILD, 0, 0, 0, 0, 0, NULL);
			pId2GetPapersFrame->ShowWindow(true, false);
			return;
		}
		//@feng 非二代验证弹出窗口
		if(msg.pSender->GetName() == _T("id2GetPapers2"))
		{ 
			Cid2GetPapers2Wnd* pId2GetPapers2Frame = new Cid2GetPapers2Wnd;
			if(pId2GetPapers2Frame == NULL) return;
			pId2GetPapers2Frame->Create(this->GetHWND(), NULL, UI_WNDSTYLE_CHILD, 0, 0, 0, 0, 0, NULL);
			pId2GetPapers2Frame->ShowWindow(true, false);
			return;
		}
		if(msg.pSender->GetName() == _T("id2compare"))            { AfxMessageBox(L"You Clicked id2compare!"); return;}
		if(msg.pSender->GetName() == _T("id2reTest_btn"))          { AfxMessageBox(L"重新采集按钮!"); return;}
		////if(msg.pSender->GetName() == _T("id2passValidity_combo"))  { AfxMessageBox(L"证件有效性下拉框!"); return;}
		////if(msg.pSender->GetName() == _T("id2passInfoReget_combo")) { AfxMessageBox(L"证件信息重采下拉框!"); return;}
		////if(msg.pSender->GetName() == _T("choiceperson_combo"))     { AfxMessageBox(L"选择考生下拉框!"); return;}
		//@feng Report菜单按钮功能
		if(msg.pSender->GetName() == _T("btn_type_choice"))  
		{ 
// 			CMenu pPopMenu;
// 			pPopMenu.CreatePopupMenu();
// 			pPopMenu.AppendMenu(MF_STRING,ID_NEWMENU+6,_T("测试菜单1"));
// 			pPopMenu.AppendMenu(MF_SEPARATOR); //分隔线
// 			pPopMenu.AppendMenu(MF_STRING,ID_NEWMENU+7,_T("测试菜单2"));
// 			pPopMenu.AppendMenu(MF_STRING,ID_NEWMENU+8,_T("测试菜单3"));
// 			pPopMenu.AppendMenu(MF_POPUP,(UINT)pPopMenu.m_hMenu,_T("测试菜单4"));
// 			pPopMenu.GetSubMenu(4)->AppendMenu(MF_STRING,ID_NEWMENU+9,_T("测试菜单4-1"));
// 			pPopMenu.GetSubMenu(4)->AppendMenu(MF_SEPARATOR); //分隔线
// 			pPopMenu.GetSubMenu(4)->AppendMenu(MF_POPUP,(UINT)pPopMenu.GetSubMenu(4)->m_hMenu,_T("测试菜单4-2"));
// 			pPopMenu.GetSubMenu(4)->GetSubMenu(2)->AppendMenu(MF_STRING,ID_NEWMENU+10,_T("测试菜单4-2-1"));
			POINT point = {msg.ptMouse.x, msg.ptMouse.y};
			::ClientToScreen(*this, &point);
			pPopMenu.TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,(CWnd*)CWnd::FromHandle(m_hWnd));
			return;	
			}
		//@feng Report人员选择对话框
		if(msg.pSender->GetName() == _T("btn_person_choice")) 
		{ 
			CDlgChoPerson dlg;
			dlg.DoModal();

			m_List_Info = dlg.St_list_Info;

			//测试返回结果
			char tim[64];
			strcpy(tim,m_List_Info.Times);
			m_RegistrNum = dlg.m_RegId;
			return;
		}
		if(msg.pSender->GetName() == _T("btn_new"))    
		{
			pListReport = static_cast<CListUI*>(m_pm.FindControl(_T("list_port")));
			if (!pListReport) return ;

			pListElementRp = new CListTextElementUI;
			pListElementRp->SetTag(1);
			pListReport->Add(pListElementRp);	
			pListElementRp->SetText(0, _T("1"));
			pListElementRp->SetText(1, _T("1"));
			pListElementRp->SetText(2, _T("1"));
			pListElementRp->SetText(3, _T("1"));
			pListElementRp->SetText(4, _T("1"));
			pListElementRp->SetText(5, _T("1"));
		//	AfxMessageBox(L"新增记录按钮!");


			//注意添加到析构
// 			if(pListElementRp!=NULL)
// 			{
// 				delete pListElementRp;
// 				pListElementRp = NULL;
// 			}
			//pUpLoadFrame->ShowModal();
			return;
		}

		if(msg.pSender->GetName() == _T("btn_save"))   
		{
			string exam_id("test");
			string guid("3");
			CString file;
			//上传文件到数据库
			for(int i =0; i < m_AddFileList.size(); i++)
			{
				file += m_AddFileList[i];
				AfxMessageBox(file);

				CStringA pathA(file);
				string path = pathA;
				 if(!g_db.AddAttachment(exam_id,guid,path));
					 return ;
			}

			//获取编辑框内容
			CRichEditUI* m_pEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("edit_more")));
			if (!m_pEdit) return ;
			m_Detail = m_pEdit->GetText();

			//获取其他信息，并上传到数据库
			time_t t= time(0);
			char Date[64];
			strftime(Date,sizeof(Date),"%Y%m%d",localtime(&t));

			EXAM_REPORT Report;
			memset(&Report,0,sizeof(Report));

			Report.Report_type = m_disp_Num;
			strcpy(Report.Occurrence_time,Date);

			CStringA people(m_RegistrNum);
// 			string na = people;
// 			string nam = string_To_UTF8(na);
			Report.People = people;

			CStringA detail(m_Detail);
			Report.Detail = detail;

			char num[50] = "8";
			strcpy(Report.Attachment_guid , num);

			//问题，上传以后数据库出现乱码
			string examId("test");
			g_db.AddEvent(examId,"1",4,Report);

			AfxMessageBox(/*L"数据已保存!"*/m_Detail); 
			return;
		}
		if(msg.pSender->GetName() == _T("btn_delete")) 
		{
			int n = pListReport->GetCurSel();
			pListReport->RemoveAt(n);
			
			AfxMessageBox(L"数据已删除!"); 
			return;
		}
		if(msg.pSender->GetName() == _T("btn_upload")) 
		{
			//@feng 上传窗口显示
			CUpLoadProgressBar* pUpLoadFrame = new CUpLoadProgressBar;
			if(pUpLoadFrame == NULL) return;
			pUpLoadFrame->Create(this->GetHWND(), NULL, UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
			pUpLoadFrame->CenterWindow();
			pUpLoadFrame->ShowModal();
			return; 
		}
		if(msg.pSender->GetName() == _T("btn_add"))
		{ 
			//@feng add按钮给Combo下拉框添加元素（例）

			CString m_strDescPath = BootOpenDialog();

			//发送路径到数据库
			m_AddFileList.push_back(m_strDescPath);

			CString strgap("\\");
			int nPos = m_strDescPath.Find(strgap);
			CString strRight("");
			while(0 <= nPos)
			{
				strRight = m_strDescPath.Left(nPos);

				m_strDescPath = m_strDescPath.Right(m_strDescPath.GetLength()-nPos -1);

				nPos = m_strDescPath.Find(strgap);
			}
			
			CComboUI* m_pCbtn_add = static_cast<CComboUI*>(m_pm.FindControl(_T("com_up")));
			CListLabelElementUI *pLabel = new CListLabelElementUI;        
			pLabel->SetAttribute(_T("text"), m_strDescPath);      
			m_pCbtn_add->Add(pLabel);
   
			return;
		}
		if(msg.pSender->GetName() == _T("btn_del"))    
		{ 
			//@feng del按钮给Combo下拉框删除元素（例）
			CComboUI* m_pCbtn_add = static_cast<CComboUI*>(m_pm.FindControl(_T("com_up")));
			m_pCbtn_add->RemoveAt(ComboIndex);
			return;
		}
		/*if( msg.pSender->GetName() == _T("menubtn") ) { AfxMessageBox(L"You Clicked Menu!"); return; }*/
		if( msg.pSender->GetName() == _T("minbtn") )   { SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
		if( msg.pSender->GetName() == _T("maxbtn") ) 
		{ 
			if (IsZoomed(*this))
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			else
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			return; 
		}
		if( msg.pSender->GetName() == _T("closebtn") ) { PostQuitMessage(0); return; }
		if( msg.pSender->GetName() == _T("id1passPhoto_btn") ){}
	}
	else if(msg.sType==_T("selectchanged"))
	{
		CDuiString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
		if(name==_T("id1check"))
			pControl->SelectItem(0);
		else if(name==_T("id2check"))
		{
			pControl->SelectItem(1);
			//同时本场考试信息显示到界面，考生信息加载到选择考生下拉框
			initID2Check();
		}
		else if(name==_T("stubook"))
			pControl->SelectItem(2);
		else if(name==_T("report"))
			pControl->SelectItem(3);
	}
	//@feng StuBooklist列表点击
	else if( msg.sType == _T("itemclick") )
	{
		//获取的鼠标点击的行
		int iIndex = msg.pSender->GetTag();
		
		// 创建数据库对象 tudo->换全局
		//InstanceDB list_db(_T("..\\DB\\VYExamDB"));

		//从数据库中获取所有的考生ID
		string exam_id = g_examId;
		vector<string> persons;
		if(!(g_db.GetRosterPersons(exam_id,persons)))
		{
			AfxMessageBox(L"获取数据库所有考生ID失败");
			return;
		}
		//定义考生信息对象
		EXAM_ROSTER exam_roster;

		//获取考生信息
		g_db.GetRosterPersonInfo(exam_id,persons[iIndex],exam_roster);

		//采集信息文本框数据
		CTextUI* stuRegisterNum = static_cast<CTextUI*>(m_pm.FindControl(_T("registerNum_text")));   stuRegisterNum->SetText((CString)exam_roster.Exam_id);
		CTextUI* stuCnName = static_cast<CTextUI*>(m_pm.FindControl(_T("cnName_text")));            stuCnName->SetText((CString)exam_roster.Full_name);
		CTextUI* stuEnName = static_cast<CTextUI*>(m_pm.FindControl(_T("enName_text")));            stuEnName->SetText((CString)exam_roster.Fix_en_name);
		CTextUI* stuCnsName = static_cast<CTextUI*>(m_pm.FindControl(_T("cnsName_text")));          stuCnsName->SetText((CString)exam_roster.Fix_chs_name);
		CTextUI* stuEnsName = static_cast<CTextUI*>(m_pm.FindControl(_T("ensName_text")));          stuEnsName->SetText((CString)exam_roster.Fix_chs_surname);
		CTextUI* stuPassType = static_cast<CTextUI*>(m_pm.FindControl(_T("passType_text")));        stuPassType->SetText((CString)exam_roster.Fix_active);
		CTextUI* stuPassNum = static_cast<CTextUI*>(m_pm.FindControl(_T("passNum_text")));          stuPassNum->SetText((CString)exam_roster.Card_num);
		CTextUI* stuExamSource = static_cast<CTextUI*>(m_pm.FindControl(_T("examSource_text")));     stuExamSource->SetText((CString)exam_roster.Born);
		CTextUI* stuBirth = static_cast<CTextUI*>(m_pm.FindControl(_T("birth_text")));              stuBirth->SetText((CString)exam_roster.Fix_born);
		CTextUI* stuPassVaildity = static_cast<CTextUI*>(m_pm.FindControl(_T("passVaildity_text"))); stuPassVaildity->SetText((CString)exam_roster.Fix_active);
		return;
	}
	else if (_tcsicmp(msg.pSender->GetName(), _T("btn_menu")) == 0)	
	{
// 		CMenuWnd* pMenu = new CMenuWnd(m_hWnd);
// 		CDuiPoint point = msg.ptMouse;
// 		ClientToScreen(m_hWnd, &point);
// 		STRINGorID xml(IDR_XML_MENU);
// 		pMenu->Init(NULL, xml, _T("xml"), point);
	}
	//@feng 下拉框.列表框消息处理
	else if(msg.sType==_T("itemselect"))
	{
		if(msg.pSender->GetName() == _T("com_evn"))   { AfxMessageBox(L"时间发生时间下拉框!"); return;}//时间空间
		if(msg.pSender->GetName() == _T("com_up"))    
		{ 
			CComboUI* m_pCbtn_add = static_cast<CComboUI*>(m_pm.FindControl(_T("com_up")));
			ComboIndex = m_pCbtn_add->GetCurSel();
			return;
		}
		
		if(msg.pSender->GetName() == _T("id2passValidity_combo"))  { AfxMessageBox(L"证件有效性下拉框!"); return;}
		if(msg.pSender->GetName() == _T("id2passInfoReget_combo")) { AfxMessageBox(L"证件信息重采下拉框!"); return;}
		if(msg.pSender->GetName() == _T("choiceperson_combo"))     
		{ 
			//1.先清空界面,buffer
			doClearID2UIandBuffer();
			//2.刷新界面,buffer
			CComboUI* m_pChosePerson = static_cast<CComboUI*>(m_pm.FindControl(_T("choiceperson_combo")));
			ComboIndex = m_pChosePerson->GetCurSel();
			if (ComboIndex == -1)
			{
				return;
			}
			CDuiString selText = m_pChosePerson->GetItemAt(ComboIndex)->GetText();
			CString selCText = CString(selText);
			int nIndex = selCText.Find('|');
			CString selRegNo;
			if (nIndex != -1)
			{
				selRegNo = selCText.Left(nIndex);
				EXAM_ROSTER examRoster;
				g_db.GetRosterPersonInfo(g_examId,string(CStringA(selRegNo)),examRoster);
				showID2UI(examRoster);
			}
			
			return;
		}
	}
}

LRESULT CMainFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	m_pm.AddPreMessageFilter(this);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("mainframe.xml"), 0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	

	Init();
	return 0;
}

LRESULT CMainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	g_oVYDevice.SetView(m_pView1);
	g_oVYDevice.SelectCamera(CVYDevice::CAM1);
	g_oVYDevice.CloseCamera();
	g_oVYDevice.SetView(m_pView2);
	g_oVYDevice.SelectCamera(CVYDevice::CAM2);
	g_oVYDevice.CloseCamera();
	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) )
		bHandled = FALSE;

	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CMainFrame::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainFrame::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainFrame::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {

			CContainerUI *pTabsContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("tabs")));
			CContainerUI *pSystemContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("systembtn")));
			CRect rt1 = pTabsContainer->GetPos();	
			CRect rt2 = pSystemContainer->GetPos();
			if (!rt1.PtInRect(pt)&&!rt2.PtInRect(pt))
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CMainFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;
	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	////TRACE("uMsg[%d] wParam[%d] lParam[%d]\n", uMsg, wParam, lParam);
	if( wParam == SC_CLOSE )
	{
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed )
	{
	}

	return lRes;
}

LRESULT CMainFrame::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	static int rnum= 1125;
	char buf[32];
	for(int i =0 ; i < m_Own_Value.size();i++)
	{
		if(i > 0)
			rnum = 1125;
		rnum += m_Own_Value[i];

		m_disp_Num = i;
// 		itoa(i,buf,32);
// 		CStringA Ord(buf);
// 		CString order(Ord.GetBuffer());
// 		Ord.ReleaseBuffer();

//		CStringA dis(m_Discription[i]);
		CString m_disp(m_Discription[i].c_str());

// 		CString Rsult;
// 		Rsult.Format(_T("点击序号为：%d，菜单值为%s"),i,disp);
		if(rnum == wParam)
		{
			AfxMessageBox(m_disp);
// 			pListReport = static_cast<CListUI*>(m_pm.FindControl(_T("list_port")));
// 			if (!pListReport) return FALSE;
// 
// 			pListElementRp = new CListTextElementUI;
// 			pListElementRp->SetTag(i);
// 			pListReport->Add(pListElementRp);	
// 			pListElementRp->SetText(0, order);
// 			pListElementRp->SetText(1, _T("test"));
// 			pListElementRp->SetText(2, _T("test"));
// 			pListElementRp->SetText(3, _T("test"));
// 			pListElementRp->SetText(4, _T("test"));
// 			pListElementRp->SetText(5, _T("test"));

			break;
		}
		else
			continue;
	}



	return S_OK;
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TRACE("uMsg[%d] wParam[%d] lParam[%d]\n", uMsg, wParam, lParam);
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) 
	{ 
	case WM_ADDLISTITEM:   lRes = OnAddListItem(uMsg, wParam, lParam, bHandled); break;
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:		   lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:	   lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_COMMAND:	   lRes = OnCommand(uMsg, wParam, lParam, bHandled); break;

	default:
		bHandled = FALSE;
	}
	if( bHandled )
		return lRes;

	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) )
		return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_KEYDOWN ) {
		if( wParam == VK_RETURN ) {
			//@feng回车注释代码
			/*CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("et_id1")));
			pEdit->SetFocus();*/
			return true;
		}
		else if( wParam == VK_ESCAPE ) {
			PostQuitMessage(0);
			return true;
		}

	}
	return false;
}

BOOL CMainFrame::InitVideo()
{
	//视频控件初始化
	CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
	pControl->SelectItem(0);

	CActiveXUI* pActiveXUI1 = static_cast<CActiveXUI*>(m_pm.FindControl(_T("id1ActiveX")));
	if( pActiveXUI1 ) 
	{
		pActiveXUI1->GetControl(IID_IUnknown, (void**)&m_pView1);
		if( m_pView1 != NULL ) 
		{
			g_oVYDevice.SetView(m_pView1);
			g_oVYDevice.SelectCamera(CVYDevice::CAM2);
			g_oVYDevice.OpenCamera();
			g_oVYDevice.ShowOn();
		}
	}

	pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
	pControl->SelectItem(1);

	CActiveXUI* pActiveXUI2 = static_cast<CActiveXUI*>(m_pm.FindControl(_T("id2ActiveX")));
	if( pActiveXUI2 ) 
	{
		pActiveXUI2->GetControl(IID_IUnknown, (void**)&m_pView2);
		if( m_pView2 != NULL ) 
		{
			g_oVYDevice.SetView(m_pView2);
			g_oVYDevice.SelectCamera(CVYDevice::CAM1);
			g_oVYDevice.OpenCamera();
			g_oVYDevice.ShowOn();
		}
	}

	pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
	pControl->SelectItem(0);

	return TRUE;
}

LRESULT CMainFrame::OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CListTextElementUI* pListElement = (CListTextElementUI*)lParam;
	CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("rosterlist")));
	if( pList ) pList->Add(pListElement);
	return 0;
}

DWORD WINAPI CMainFrame::Search(LPVOID lpParameter)
{
	try
	{
		struct Prama* prama = (struct Prama *)lpParameter;
		CListUI* pList = prama->pList;
		//-------------------------------------
		/*
		* 添加数据循环
		*/
		for(int i=0; i<100; i++)
		{
			std::stringstream ss;
			ss << "www." << i << ".com";
			ss.clear();
			ss << "it's " << i;
			CListTextElementUI* pListElement = new CListTextElementUI;
			pListElement->SetTag(i);
			if (pListElement != NULL)
			{
				::PostMessage(prama->hWnd, WM_ADDLISTITEM, 0L, (LPARAM)pListElement);
			}
			/*
			*	Sleep 为了展示添加的动态效果，故放慢了添加速度，同时可以看到添加过程中界面仍然可以响应
			*/
			//::Sleep(100);
		}
		//------------------------------------------
		delete prama;
		
		return 0;
	}
	catch(...)
	{
		return 0;
	}
}
//@feng Stubooklist初始化和获取数据库数据 Reportlist初始化
BOOL CMainFrame::InitList()
{
	struct Prama *prama = new Prama;
	CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("rosterlist")));
	pList->RemoveAll();
	if (!pList) return FALSE;

	// 创建数据库对象 tudo->换全局
	//InstanceDB list_db(_T("..\\DB\\VYExamDB"));

	//从数据库中获取所有的考生ID
	string exam_id = string(g_examId);
	vector<string> persons;
	if (exam_id == "")
	{
		AfxMessageBox(L"获取数据库唯一标识是吧");
		return FALSE;
	}
	if(!(g_db.GetRosterPersons(exam_id,persons)))
	{
		AfxMessageBox(L"获取数据库所有考生ID失败");
		return FALSE;
	}
	//定义考生信息对象
	EXAM_ROSTER exam_roster;
	for (int i = 0; i < 3; i++)
	{
		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->SetTag(i);
		pList->Add(pListElement);	
		//根据考生ID获取考生信息
		g_db.GetRosterPersonInfo(exam_id,persons[i],exam_roster);
		pListElement->SetText(1, (CString)exam_roster.Full_name);
		pListElement->SetText(2, (CString)exam_roster.Fix_active);
		pListElement->SetText(3, (CString)exam_roster.Fix_register_id);
		pListElement->SetText(4, (CString)exam_roster.Fix_born);
		pListElement->SetText(5, (CString)exam_roster.Fix_chs_name);
		pListElement->SetText(6, (CString)exam_roster.Fix_chs_name);
		pListElement->SetText(7, (CString)exam_roster.Fix_chs_name);
		pListElement->SetText(8, (CString)exam_roster.Fix_chs_name);
	}
	//@feng 考场报告list初始化
	CListUI* pListReport = static_cast<CListUI*>(m_pm.FindControl(_T("list_port")));
	if (!pListReport) return FALSE;
	for (int j = 0; j < 20; j++)
	{
		CListTextElementUI* pListElementRp = new CListTextElementUI;
		pListElementRp->SetTag(j);
		pListReport->Add(pListElementRp);	
		pListElementRp->SetText(0, _T("test"));
		pListElementRp->SetText(1, _T("test"));
		pListElementRp->SetText(2, _T("test"));
		pListElementRp->SetText(3, _T("test"));
		pListElementRp->SetText(4, _T("test"));
		pListElementRp->SetText(5, _T("test"));
	}
	return TRUE;
}

/*
* 关键的回调函数，IListCallbackUI 中的一个虚函数，渲染时候会调用,在[1]中设置了回调对象
*/
LPCTSTR CMainFrame::GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
{
	//TRACE("pControl=%d iIndex=%d iSubItem=%d \n", pControl, iIndex, iSubItem);
	TCHAR szBuf[MAX_PATH] = {0};
	switch (iSubItem)
	{
	case 0:
		_stprintf(szBuf, _T("%d"), iIndex);
		break;
	case 1:
		{
#ifdef _UNICODE		
// 			int iLen = domain[iIndex].length();
// 			LPWSTR lpText = new WCHAR[iLen + 1];
// 			::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
// 			::MultiByteToWideChar(CP_ACP, 0, domain[iIndex].c_str(), -1, (LPWSTR)lpText, iLen) ;
// 			_stprintf(szBuf, lpText);
// 			delete[] lpText;
			_stprintf(szBuf, _T("%s"), _T("测试数据"));
#else
			_stprintf(szBuf, domain[iIndex].c_str());
#endif
		}
		break;
	case 2:
		{
#ifdef _UNICODE		
// 			int iLen = desc[iIndex].length();
// 			LPWSTR lpText = new WCHAR[iLen + 1];
// 			::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
// 			::MultiByteToWideChar(CP_ACP, 0, desc[iIndex].c_str(), -1, (LPWSTR)lpText, iLen) ;
// 			_stprintf(szBuf, lpText);
// 			delete[] lpText;
			_stprintf(szBuf, _T("%s"), _T("测试数据"));
#else
			_stprintf(szBuf, desc[iIndex].c_str());
#endif
		}
		break;
	case 3:
		{
#ifdef _UNICODE		
			_stprintf(szBuf, _T("%s"), _T("测试数据"));
#else
			_stprintf(szBuf, desc[iIndex].c_str());
#endif
		}
		break;
	case 4:
		{
#ifdef _UNICODE		
			_stprintf(szBuf, _T("%s"), _T("测试数据"));
#else
			_stprintf(szBuf, desc[iIndex].c_str());
#endif
		}
		break;
	case 5:
		{
#ifdef _UNICODE		
			_stprintf(szBuf, _T("%s"), _T("测试数据"));
#else
			_stprintf(szBuf, desc[iIndex].c_str());
#endif
		}
		break;
	case 6:
		{
#ifdef _UNICODE		
			_stprintf(szBuf, _T("%s"), _T("测试数据"));
#else
			_stprintf(szBuf, desc[iIndex].c_str());
#endif
		}
		break;
	case 7:
		{
#ifdef _UNICODE		
			_stprintf(szBuf, _T("%s"), _T("测试数据"));
#else
			_stprintf(szBuf, desc[iIndex].c_str());
#endif
		}
		break;
	}
	pControl->SetUserData(szBuf);
	return pControl->GetUserData();
}




CString CMainFrame::BootOpenDialog()   //返回选择的文件名称
{
	CString strFile = _T("");

	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.cfg)|*.cfg|All Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}


	CString strgap("\\");
	int nPos = strFile.Find(strgap);
	CString strRight("");
	while(0 <= nPos)
	{
		strRight = strFile.Left(nPos);

		strFile = strFile.Right(strFile.GetLength()-nPos -1);

		nPos = strFile.Find(strgap);
	}

	return strFile;
}

bool CMainFrame::InitMenu()
{
	//创建按钮
	pPopMenu.CreatePopupMenu();

	//获取本次考试事件类型版本
	string examid("test");
	VERSION_CONTROL version_control;
	if(!g_db.QueryVersionControl(examid, version_control))
		return false;
	//获取根节点数据
	std::vector<EventNode> nodes;
	if(!g_db.FetchChildEvent(version_control.Event_type_version, 0, nodes))
		return false;
	//调用递归遍历
	Recurrence(version_control.Event_type_version, nodes, &pPopMenu);

	return true;
}


void CMainFrame::Recurrence(string ver,vector<EventNode>& nodes,CMenu *pMenu)
{
	int i =0;
	for(vector<EventNode>::iterator iter = nodes.begin();iter != nodes.end();iter++,i++)
	{
		//格式转换
		CStringA DesvA((*iter).Dsec);
		CString Desc(DesvA.GetBuffer());
		DesvA.ReleaseBuffer();

		m_Own_Value.push_back((*iter).Own_value);
		m_Discription.push_back((*iter).Dsec);

		//获取子事件
		vector<EventNode> childNodes;
		g_db.FetchChildEvent(ver, (*iter).Own_value, childNodes);
		if(childNodes.size() == 0)
		{
			pMenu->AppendMenu(MF_STRING,ID_NEWMENU+(*iter).Own_value, Desc);
			if(iter != nodes.end() - 1)    //如果是最后一个，就不需要分割线
				pMenu->AppendMenu(MF_SEPARATOR);
		}
		else
		{
			pMenu->AppendMenu(MF_POPUP,(UINT)pMenu->m_hMenu, Desc);
			if(iter != nodes.end() - 1)
				pMenu->AppendMenu(MF_SEPARATOR); 
			//递归实现
			Recurrence(ver, childNodes, pMenu->GetSubMenu(i*2));
		}

		childNodes.clear();
	}

	nodes.clear();
}


bool CMainFrame::ShowReport()
{
	string examId("test");
	vector<EXAM_REPORT> events;
	if(!g_db.QueryEvent(examId,events,true))
		return false;

	int num = 0;
	char buf[32],Buf[32];
	for(vector<EXAM_REPORT>::iterator iter = events.begin();iter != events.end();iter++,num++)
	{
		itoa(num,buf,32);
		
		CStringA TT(buf);
		CString Num(TT.GetBuffer());
		TT.ReleaseBuffer();

		itoa((*iter).Report_type,Buf,32);
		CStringA T1(Buf);
		CString Type(T1.GetBuffer());
		T1.ReleaseBuffer();

		CStringA T2((*iter).Occurrence_time);
		CString Dates(T2.GetBuffer());
		T2.ReleaseBuffer();

		CStringA T3((*iter).People.c_str());
		CString people(T3.GetBuffer());
		T3.ReleaseBuffer();

		CStringA T4((*iter).Detail.c_str());
		CString detial(T4.GetBuffer());
		T4.ReleaseBuffer();

		CStringA T5((*iter).Attachment_guid);
		CString guids(T5.GetBuffer());
		T5.ReleaseBuffer();

		pListReport = static_cast<CListUI*>(m_pm.FindControl(_T("list_port")));
		if (!pListReport) return FALSE;

			pListElementRp = new CListTextElementUI;
			pListElementRp->SetTag(num);
			pListReport->Add(pListElementRp);	
			pListElementRp->SetText(0, Num);
			pListElementRp->SetText(1, Type);
			pListElementRp->SetText(2, Dates);
			pListElementRp->SetText(3, people);
			pListElementRp->SetText(4, detial);
			pListElementRp->SetText(5, guids);

	}
	events.clear();

	return true;
}
