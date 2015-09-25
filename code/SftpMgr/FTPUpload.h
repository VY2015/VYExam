#ifndef __FTP_UPLOAD_H__
#define __FTP_UPLOAD_H__

#ifndef __VY_SFTP_DEF_H__
#include "vysftpdef.h"
#endif

#include <list>

class CCurlInst;

class  __declspec(dllexport) CFTPUpload
{
public:
    CFTPUpload();
    virtual ~CFTPUpload();

	bool										Init(const FtpConfigData& stCfgData);

    bool										IsEffective();	//是否有效(链接和有无数据)
    void                                        ResetFTP();

    bool                                        Upload(std::string strExamTime, std::string strExamRoom, SFTP_PROGRESS_CALLBACK_FUNC progressFunc = NULL, char* ptr = NULL);
	bool										DownLoad(std::string strExamRoom, SFTP_PROGRESS_CALLBACK_FUNC progressFunc = NULL, char* ptr = NULL);

	bool										GetExamTime(std::string& strExamTime);
    bool                                        GetExamVersion(std::string& strExamVersion);

    eVYSFTP_PROC_STATUS                         GetSftpProcStatus();

private:
	bool                                        UploadFile(std::string strUploadPath = "", bool bPic = false, SFTP_PROGRESS_CALLBACK_FUNC progressFunc = NULL, char* ptr = NULL);
	bool                                        DownLoadFile(std::string strUploadPath = "", bool bVersion = false, bool bPic = false, SFTP_PROGRESS_CALLBACK_FUNC progressFunc = NULL, char* ptr = NULL);
    
    bool                                        FillData(std::string strLocalFile = "", bool bPic = false);
    bool                                        FillServerFile(const std::string& strFileName = "", bool bVersion = false, bool bPic = false);
    void                                        ParseLocalFiles(std::string strFilePath);
    
    bool                                        SetExamDate(std::string strExamDatePath);
    
    bool                                        SetExamVersion(std::string strExamVersionPath);
    bool                                        SetLastExamVersion(std::string strLastExamVersionPath);

	bool										CreateDir(char* pszDir);
    

private:
    FtpEnv										m_stFtpEnv;
    FileAttr									m_stLocalFileAttr;
    std::string									m_strLocalPath;
    std::string									m_strLocalBakPath;
    std::list<std::string>                      m_lstLocalFiles;
    std::string                                 m_strExamDate;
    std::string                                 m_strExamRoom;

    std::string                                 m_strExamVersion;
    std::string                                 m_strLastExamVersion;

    eVYSFTP_PROC_STATUS                         m_eVYSftpProcStatus;

    std::list<FileAttr >                        m_lstUploadFailedFile;

private:
    CCurlInst*									m_pCurlInst;			//Ftp操作
};
#endif
