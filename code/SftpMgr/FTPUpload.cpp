#ifndef __FTP_UPLOAD_H__
#include "FTPUpload.h"
#endif

#ifndef _CURL_INST_H_
#include "CurlInst.h"
#endif

#include <direct.h>
#include <string.h>
#include <sys/stat.h>

//#include <unistd.h>
#include <windows.h>

#include <io.h>
#include <iostream>

#include "encryptyFile.h"

CFTPUpload::CFTPUpload()
    : m_strLocalPath("")
    , m_strLocalBakPath("")
    , m_strExamDate("")
    , m_strExamRoom("")
    , m_strExamVersion("")
    , m_strLastExamVersion("")
    , m_eVYSftpProcStatus(SFTP_SYN_STATUS_ERR)
    , m_pCurlInst(NULL)
{
    m_stFtpEnv._strHostName			= "";
    m_stFtpEnv._strUserName			= "";
    m_stFtpEnv._strPassword			= "";
    m_stFtpEnv._strFtpUploadPath    = "";
    m_stFtpEnv._strFtpDownloadPath  = "";
    m_stFtpEnv._strPort				= "22";

    m_stLocalFileAttr._fp			= NULL;
    m_stLocalFileAttr._nOffset      = 0;
    m_stLocalFileAttr._strFileName  = "";

    m_lstLocalFiles.clear();
    m_lstUploadFailedFile.clear();
}

CFTPUpload::~CFTPUpload()
{
	if (CCurlInst::GetInstance())
	{
		delete CCurlInst::GetInstance();
	}
	
    m_lstLocalFiles.clear();
    m_lstUploadFailedFile.clear();
}

bool CFTPUpload::Init(const FtpConfigData& stCfgData)
{
    m_stFtpEnv._strHostName         = stCfgData._stFtpActive._strHostName;
    m_stFtpEnv._strUserName         = stCfgData._stFtpActive._strUserName;
    m_stFtpEnv._strPassword         = stCfgData._stFtpActive._strPassword;
    m_stFtpEnv._strPort             = stCfgData._stFtpActive._strPort;
    m_stFtpEnv._strFtpUploadPath    = stCfgData._stFtpActive._strFtpUploadPath;
    m_stFtpEnv._strFtpDownloadPath  = stCfgData._stFtpActive._strFtpDownloadPath;

    m_strLocalPath                  = stCfgData._stLocalPath._strLocalPath;
    m_strLocalBakPath               = stCfgData._stLocalPath._strLocalBakPath;

    m_pCurlInst = CCurlInst::GetInstance();
    if(NULL == m_pCurlInst)
        return false;

    if(!m_pCurlInst->Init(m_stFtpEnv))
        return false;

    return true;
}

bool CFTPUpload::IsEffective()
{
    m_eVYSftpProcStatus = SFTP_CONNECT_STATUS_FIRST_START;

    if (m_pCurlInst->IsConnect())
    {
        m_eVYSftpProcStatus = SFTP_CONNECT_STATUS_FIRST_SUCCESS;
        return true;
    }

    m_eVYSftpProcStatus = SFTP_CONNECT_STATUS_FIRST_FAILURE;
    
    Sleep(100);

    m_eVYSftpProcStatus = SFTP_CONNECT_STATUS_SECOND_START;

    if (m_pCurlInst->IsConnect())
    {
        m_eVYSftpProcStatus = SFTP_CONNECT_STATUS_SECOND_SUCCESS;
        return true;
    }

    m_eVYSftpProcStatus = SFTP_CONNECT_STATUS_SECOND_FAILURE;

    return false;
}

void CFTPUpload::ResetFTP()
{
    m_pCurlInst->Reset();
}

bool CFTPUpload::Upload(std::string strExamTime, std::string strExamRoom, SFTP_PROGRESS_CALLBACK_FUNC progressFunc, char* ptr)
{
    m_strExamDate = strExamTime;
    m_strExamRoom = std::string("/") + strExamRoom + std::string("/");

    //upload roster
    if (!UploadFile(m_strLocalPath + m_strExamDate + m_strExamRoom, false, progressFunc, ptr))
        return false;

    //upload pic
    if (!UploadFile(m_strLocalPath + m_strExamDate + m_strExamRoom, true, progressFunc, ptr))
        return false;

    return true;
}

bool  CFTPUpload::UploadFile(std::string strUploadPath, bool bPic, SFTP_PROGRESS_CALLBACK_FUNC progressFunc, char* ptr)
{
    if(bPic)
    {
        strUploadPath += "curImages/";
    }

    ParseLocalFiles(strUploadPath);

    int nUploadCount = 0;
    for(std::list<std::string>::iterator iter = m_lstLocalFiles.begin();
		iter != m_lstLocalFiles.end(); iter++)
    {
        if(!FillData(*iter, bPic))
        {
            continue;
        }

		if (!m_pCurlInst->Upload(m_stLocalFileAttr, progressFunc, ptr))
        {
            m_lstUploadFailedFile.push_back(m_stLocalFileAttr);
        }
        else
        {
            nUploadCount++;
        }

        fclose(m_stLocalFileAttr._fp);
        m_stLocalFileAttr._fp = NULL;

		////备份文件
		//std::string strBakFile;
		//std::string::size_type nPos = (*iter).rfind('/');
		//strBakFile.assign((*iter), nPos + 1, (*iter).length() - nPos - 1);
		//	
		//std::string strBakPath;			
		//if(bPic)
		//{
		//	strBakPath = m_strLocalBakPath + m_strExamDate + m_strExamRoom + "pic/" + strBakFile;
		//}
		//else
		//{
		//	strBakPath = m_strLocalBakPath + m_strExamDate + m_strExamRoom + strBakFile;
		//}
		//	
		////创建目录
		//size_t nBakPos = strBakPath.rfind(strBakFile);
		//if (std::string::npos == nBakPos)
		//	return;
		//	
		//char pBakDir[255] = { 0 };
		//std::string strBakPath1 = strBakPath.substr(0, nBakPos);
		//strcpy_s(pBakDir, strBakPath1.c_str());
		//	
		//CreateDir(pBakDir);
		//	
		//if (0 != rename((*iter).c_str(), strBakPath.c_str()))
		//{
		//	perror("rename error");
		//}		
    }

	//_rmdir((m_strLocalPath + m_strExamDate + m_strExamRoom + "/pic").c_str());
	//_rmdir((m_strLocalPath + m_strExamDate + m_strExamRoom).c_str());

    if (nUploadCount < m_lstLocalFiles.size())
    {
        int nIndex = 3;
        while (nIndex > 0)
        {
            for (std::list<FileAttr>::iterator iter = m_lstUploadFailedFile.begin();
                iter != m_lstUploadFailedFile.end();)
            {
                if (!m_pCurlInst->Upload(m_stLocalFileAttr, progressFunc, ptr))
                {
                    iter++;
                }
                else
                {
                    iter = m_lstUploadFailedFile.erase(iter);
                }
            }

            nIndex--;
        }

        //写文件
        FILE* fp = NULL;
        fopen_s(&fp, "uploaderr.txt", "w+");
        char filePath[256] = {0};

        for (std::list<FileAttr>::iterator iter = m_lstUploadFailedFile.begin();
            iter != m_lstUploadFailedFile.end(); iter++)
        {
            sprintf_s(filePath, "%s\n", (*iter)._strFileName);
            fprintf_s(fp, filePath);
        }

        fclose(fp);
        fp = NULL;
 
        if (m_lstUploadFailedFile.size() > 0)
            return false;
    }

    return true;
}

bool CFTPUpload::DownLoad(std::string strExamRoom, SFTP_PROGRESS_CALLBACK_FUNC progressFunc, char* ptr)
{
    m_eVYSftpProcStatus = SFTP_DATE_VERSION_STATUS_CHECK_START;

	m_strExamRoom = "/" + strExamRoom + "/";

	//获取考试时间
    if (!DownLoadFile(m_stFtpEnv._strFtpDownloadPath, true, false, progressFunc, ptr))
    {
        m_eVYSftpProcStatus = SFTP_DATE_VERSION_STATUS_CHECK_FAILURE;
        return false;
    }

    if (!SetExamDate(m_strLocalPath))
    {
        m_eVYSftpProcStatus = SFTP_DATE_VERSION_STATUS_CHECK_FAILURE;
        return false;
    }

    m_eVYSftpProcStatus = SFTP_DATE_VERSION_STATUS_CHECK_SUCCESS;
    
    //获取花名册版本
    m_eVYSftpProcStatus = SFTP_ROSTER_VERSION_STATUS_CHECK_START;

    if (!DownLoadFile(m_stFtpEnv._strFtpDownloadPath + m_strExamDate + m_strExamRoom, true, false, progressFunc, ptr))
    {
        m_eVYSftpProcStatus = SFTP_ROSTER_VERSION_STATUS_CHECK_FAILURE;
        return false;
    }


    //当前花名册版本
    if (!SetExamVersion(m_strLocalPath + m_strExamDate + m_strExamRoom))
    {
        m_eVYSftpProcStatus = SFTP_ROSTER_VERSION_STATUS_CHECK_FAILURE;
        return false;
    }

    m_eVYSftpProcStatus = SFTP_ROSTER_VERSION_STATUS_CHECK_SUCCESS;

    //上次花名册版本
    SetLastExamVersion(m_strLocalBakPath + m_strExamDate + m_strExamRoom);

    if (m_strLastExamVersion.empty() || m_strExamVersion > m_strLastExamVersion)
    {
        m_eVYSftpProcStatus = SFTP_ROSTER_STATUS_START;

        //获取花名册
        if (!DownLoadFile(m_stFtpEnv._strFtpDownloadPath, false, false, progressFunc, ptr))
        {
            m_eVYSftpProcStatus = SFTP_ROSTER_STATUS_FAILURE;
            return false;
        }

        m_eVYSftpProcStatus = SFTP_ROSTER_STATUS_SUCCESS;

        //获取图片
        
        m_eVYSftpProcStatus = SFTP_HISTORY_STATUS_START;

        if (!DownLoadFile(m_stFtpEnv._strFtpDownloadPath, false, true, progressFunc, ptr))
        {
            m_eVYSftpProcStatus = SFTP_HISTORY_STATUS_FAILURE;
        }

        m_eVYSftpProcStatus = SFTP_HISTORY_STATUS_SUCCESS;
    }
    else
    {
        m_eVYSftpProcStatus = SFTP_VERSION_STATUS_CHECK_NO_PASS;
    }

    //创建目录
    char pDir[255] = { 0 };
	strcpy_s(pDir, sizeof(pDir), (m_strLocalBakPath + m_strExamDate + m_strExamRoom).c_str());
    CreateDir(pDir);

    CopyFileA((m_strLocalPath + m_strExamDate + m_strExamRoom + "rosterVersion.txt").c_str(),
        (m_strLocalBakPath + m_strExamDate + m_strExamRoom + "rosterVersion.txt").c_str(), false);

    return true;
}

bool CFTPUpload::DownLoadFile(std::string strUploadPath, bool bVersion, bool bPic, SFTP_PROGRESS_CALLBACK_FUNC progressFunc, char* ptr)
{
    std::list<std::string> lstFoundFiles;
    if(bVersion)
    {

    }
    else if(bPic)
    {
        strUploadPath = strUploadPath + m_strExamDate + m_strExamRoom + "/pic/";
    }
    else
    {
        strUploadPath = strUploadPath + m_strExamDate + m_strExamRoom;
    }

    m_pCurlInst->GetSvrFiles(strUploadPath, lstFoundFiles, bVersion);
    if(lstFoundFiles.empty())
        return false;

    int nDownCount = 0;
    for(std::list<std::string>::const_iterator iter = lstFoundFiles.begin(); iter != lstFoundFiles.end(); iter++)
    {
        if (!FillServerFile(*iter, bVersion, bPic))
        {
            continue;
        }

		bool bDownRet = m_pCurlInst->DownLoad(m_stLocalFileAttr, progressFunc, ptr);
		fclose(m_stLocalFileAttr._fp);
		m_stLocalFileAttr._fp = NULL;

        if (bDownRet)
        {
            nDownCount++;
        }


		//解密
		if (bPic)
		{
			size_t nPos = m_stLocalFileAttr._strFileName.rfind('/');
			std::string strJpgPath = m_stLocalFileAttr._strFileName.substr(nPos + 1, m_stLocalFileAttr._strFileName.length() - nPos - 4);
			strJpgPath = m_strLocalPath + m_strExamDate + m_strExamRoom + "hisImages/" + strJpgPath + "jpg";

			std::string strBinPath = m_stLocalFileAttr._strFileName.substr(nPos + 1, m_stLocalFileAttr._strFileName.length() - nPos - 1);
			strBinPath = m_strLocalPath + m_strExamDate + m_strExamRoom + "hisPic/" + strBinPath;

			std::string key("justcoolveryescn");

			CEncryptyFile cEncryptyFile;
			cEncryptyFile.AesEncryptorDecryptFile(strBinPath, key, strJpgPath);
		}
    }

    if (nDownCount < lstFoundFiles.size())
        return false;

    return true;
}


bool CFTPUpload::FillData(std::string strLocalFile, bool bPic)
{
    struct stat file_info;

    std::string::size_type nPos = strLocalFile.rfind('/');

    std::string strFileName;
    strFileName.assign(strLocalFile, nPos + 1, strLocalFile.length() - nPos - 1);

    if(0 != stat(strLocalFile.c_str(), &file_info))
    {
        perror("stat err");//err
    }

    //fill pic
    if(bPic)
    {
        m_stLocalFileAttr._strFileName = m_stFtpEnv._strFtpUploadPath + m_strExamDate + m_strExamRoom + "pic/" + strFileName;
    }
    else
    {
        m_stLocalFileAttr._strFileName = m_stFtpEnv._strFtpUploadPath + m_strExamDate + m_strExamRoom + strFileName;
    }

    m_stLocalFileAttr._nOffset = file_info.st_size;

	fopen_s(&m_stLocalFileAttr._fp, strLocalFile.c_str(), "rb");

    if(NULL == m_stLocalFileAttr._fp)
    {
        return false;
    }

    return true;
}

bool CFTPUpload::FillServerFile(const std::string& strFileName, bool bVersion, bool bPic)
{
    std::string m_strLocalFileName;

    if(bVersion)
    {
        m_strLocalFileName = m_strLocalPath + strFileName;

        if (std::string::npos != strFileName.find("rosterVersion"))
        {
            m_strLocalFileName = m_strLocalPath + m_strExamDate + m_strExamRoom + strFileName;
        }
    }
    else if(bPic)
    {
        m_strLocalFileName = m_strLocalPath + m_strExamDate + m_strExamRoom + "hisPic/" + strFileName;
        _mkdir(std::string(m_strLocalPath + m_strExamDate + m_strExamRoom + "hisImages/").c_str());
    }
    else
    {
        m_strLocalFileName = m_strLocalPath + m_strExamDate + m_strExamRoom  + strFileName;
    }

	size_t nPos = m_strLocalFileName.rfind(strFileName);
	if (std::string::npos == nPos)
		return false;

	//创建历史目录
	char pDir[255] = { 0 };
	std::string strLocalDir = m_strLocalFileName.substr(0, nPos);
	strcpy_s(pDir, sizeof(pDir), strLocalDir.c_str());

    CreateDir(pDir);

    if (!m_strExamDate.empty())
    {
        //创建当前目录
		strcpy_s(pDir, sizeof(pDir), (m_strLocalPath + m_strExamDate + m_strExamRoom + "curImages/").c_str());
        CreateDir(pDir);

		strcpy_s(pDir, sizeof(pDir), (m_strLocalPath + m_strExamDate + m_strExamRoom + "curPic/").c_str());
        CreateDir(pDir);

    }
 
    //采用追加方式打开文件，便于实现文件断点续传工作
	fopen_s(&m_stLocalFileAttr._fp, m_strLocalFileName.c_str(), "wb");
    if(NULL == m_stLocalFileAttr._fp)
        return false;

    if(bVersion)
    {
        m_stLocalFileAttr._strFileName = m_stFtpEnv._strFtpDownloadPath + strFileName;

        if (std::string::npos != strFileName.find("rosterVersion"))
        {
            m_stLocalFileAttr._strFileName = m_stFtpEnv._strFtpDownloadPath + m_strExamDate + m_strExamRoom + strFileName;
        }

    }
    else if(bPic)
    {
        m_stLocalFileAttr._strFileName = m_stFtpEnv._strFtpDownloadPath + m_strExamDate + m_strExamRoom + "pic/" + strFileName;
    }
    else
    {
        m_stLocalFileAttr._strFileName = m_stFtpEnv._strFtpDownloadPath + m_strExamDate + m_strExamRoom + strFileName;
    }

    return true;
}

void CFTPUpload::ParseLocalFiles(std::string path)
{
	//文件句柄  
	long   hFile = 0;

	//文件信息  
	struct _finddata_t fileinfo;
	std::string p;

	m_lstLocalFiles.clear();

	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之 如果不是,加入列表
			//if ((fileinfo.attrib   &   _A_SUBDIR))
			//{
			//	if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
			//		ParseLocalFiles(p.assign(path).append("/").append(fileinfo.name));
			//}
			//else
			//{
			//	m_lstLocalFiles.push_back(p.assign(path).append("/").append(fileinfo.name));
			//}

			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
			{
                //查找jpg图片，且不包含_cur.jpg图片
				std::string strLocal = p.assign(path).append("/").append(fileinfo.name);
				if (std::string::npos != std::string(fileinfo.name).find(".jpg") 
                    && std::string::npos == std::string(fileinfo.name).find("_cur"))
				{
					//加密
					std::string key("justcoolveryescn");
					
					std::string strPicPath = strLocal;
					size_t nPos = strLocal.find("curImages");
					strPicPath.replace(nPos, 9, "curPic");
					nPos = strPicPath.rfind('.');
					strPicPath.replace(nPos + 1, 3, "bin");

					CEncryptyFile cEncryptyFile;
					cEncryptyFile.AesEncryptorDecryptFile(strLocal, key, strPicPath, true);
					strLocal = strPicPath;
				}
				
                if (std::string::npos != strLocal.find(".bin"))
                {
                    m_lstLocalFiles.push_back(strLocal);
                }
					
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}

bool CFTPUpload::SetExamDate(std::string strExamDatePath)
{
	FILE* fp = NULL;
	fopen_s(&fp, (strExamDatePath + "version.txt").c_str(), "rb");
	if (NULL == fp)
		return false;

    char buf[32] = {0};
    fread(buf, sizeof(buf), 1, fp);
    fclose(fp);
    fp = NULL;

    m_strExamDate = std::string(buf);

	return true;
}

bool CFTPUpload::SetExamVersion(std::string strExamVersionPath)
{
    FILE* fp = NULL;
    fopen_s(&fp, (strExamVersionPath + "rosterVersion.txt").c_str(), "rb");
    if (NULL == fp)
        return false;

    char buf[64] = { 0 };
    fread(buf, sizeof(buf), 1, fp);
    fclose(fp);
    fp = NULL;

    m_strExamVersion = std::string(buf);

    return true;
}

bool CFTPUpload::SetLastExamVersion(std::string strLastExamVersionPath)
{
    FILE* fp = NULL;
    fopen_s(&fp, (strLastExamVersionPath + "rosterVersion.txt").c_str(), "rb");
    if (NULL == fp)
        return false;

    char buf[64] = { 0 };
    fread(buf, sizeof(buf), 1, fp);
    fclose(fp);
    fp = NULL;

    m_strLastExamVersion = std::string(buf);

    return true;
}

bool CFTPUpload::GetExamTime(std::string& strExamTime)
{
	strExamTime = m_strExamDate;
	return true;
}

bool CFTPUpload::GetExamVersion(std::string& strExamVersion)
{
    strExamVersion = m_strExamVersion;
    return true;
}

eVYSFTP_PROC_STATUS CFTPUpload::GetSftpProcStatus()
{
    return m_eVYSftpProcStatus;
}


bool CFTPUpload::CreateDir(char* pszDir)
{
	int iRet = 0;
	int iLen = strlen(pszDir);

	//在末尾加/
	if (pszDir[iLen - 1] != '\\' && pszDir[iLen - 1] != '/')
	{
		pszDir[iLen] = '/';
		pszDir[iLen + 1] = '\0';
	}

	// 创建目录
	for (int i = 0; i <= iLen; i++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{
			pszDir[i] = '\0';

			//如果不存在,创建
			if (0 != _access(pszDir, 0))
			{
				if (0 != _mkdir(pszDir))
				{
					return false;
				}
			}
			//支持linux,将所有\换成/
			pszDir[i] = '/';
		}
	}
	return true;
}

