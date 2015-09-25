#ifndef _CURL_INST_H_
#define _CURL_INST_H_

#ifndef __VY_SFTP_DEF_H__
#include "vysftpdef.h"
#endif

#include <curl/curl.h>
#include <list>

class CCurlInst
{
public:
    ~CCurlInst();
    static	CCurlInst*		GetInstance();

    bool                                Init(const FtpEnv& stFtpEnv);

private:
    CCurlInst();
    static	CCurlInst*		m_Instance;

public:
    bool                                IsConnect();
    void								Reset();

	bool                                Upload(FileAttr& stFileAttr, SFTP_PROGRESS_CALLBACK_FUNC progressFunc = NULL, void* ptr = NULL);
	bool                                DownLoad(FileAttr& stFileAttr, SFTP_PROGRESS_CALLBACK_FUNC progressFunc = NULL, void* ptr = NULL);
    bool                                GetSvrFiles(std::string strServerPath, std::list<std::string>& lstFoundFiles, bool bVersion = false);

private:
    static size_t                       ReadSerFiles(void *ptr, size_t size, size_t nmemb, void *stream);
    static size_t                       WriteLocalData(void *ptr, size_t size, size_t nmemb, void *stream);  //д�����ļ�����
    static size_t                       ParserSvrFiles(void *ptr, size_t size, size_t nmemb, void *data);    //�����������ļ�
	//static int							my_progress_func(char *progress_data, double t, /* dltotal */ double d, /* dlnow */ double ultotal, double ulnow);

private:
    std::string                         m_strURL;                   //ftp����������
    std::string                         m_strUserPwd;               //ftp�������û�����

    std::string                         m_strSvrFileName;
    static std::list<std::string>       m_lstFoundFiles;            //�������ص��ļ�

};
#endif
