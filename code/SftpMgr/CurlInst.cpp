#ifndef _CURL_INST_H_
#include "CurlInst.h"
#endif


#include <iostream>

CCurlInst* CCurlInst::m_Instance = NULL;
std::list<std::string> CCurlInst::m_lstFoundFiles;

CCurlInst::CCurlInst()
    : m_strURL("")
    , m_strUserPwd("")
{
    m_lstFoundFiles.clear();
}

CCurlInst::~CCurlInst()
{
    curl_global_cleanup();
    m_lstFoundFiles.clear();

}

CCurlInst* CCurlInst::GetInstance()
{
    if (NULL == m_Instance)
    {
        m_Instance = new CCurlInst();
        CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
        if(CURLE_OK != res)
        {
            delete m_Instance;
            m_Instance = NULL;
        }
    }

    return m_Instance;
}

bool CCurlInst::Init(const FtpEnv& stFtpEnv)
{
    m_strURL  = "sftp://";
    m_strURL += stFtpEnv._strHostName;
    m_strURL += ":";
    m_strURL += stFtpEnv._strPort;

    m_strUserPwd  = stFtpEnv._strUserName;
    m_strUserPwd += ":";
    m_strUserPwd += stFtpEnv._strPassword;

    return true;
}

bool CCurlInst::IsConnect()
{
    CURL *pCurl = curl_easy_init();

    if (pCurl)
    {
        curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 0);
        curl_easy_setopt(pCurl, CURLOPT_NOBODY, 1);
        curl_easy_setopt(pCurl, CURLOPT_URL, m_strURL.c_str());
        curl_easy_setopt(pCurl, CURLOPT_USERPWD, m_strUserPwd.c_str());
        curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 60L);
        curl_easy_setopt(pCurl, CURLOPT_FORBID_REUSE, 1);
        CURLcode res = curl_easy_perform(pCurl);
        curl_easy_cleanup(pCurl);

        if (CURLE_OK == res)
            return true;
    }

    return false;
}

void CCurlInst::Reset()
{
    CURL *pCurl = curl_easy_init();
    curl_easy_reset(pCurl);
    curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(pCurl, CURLOPT_NOBODY, 1);
    curl_easy_setopt(pCurl, CURLOPT_URL, m_strURL.c_str());
    curl_easy_setopt(pCurl, CURLOPT_USERPWD, m_strUserPwd.c_str());
    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(pCurl, CURLOPT_FORBID_REUSE, 1);
    CURLcode res = curl_easy_perform(pCurl);
    curl_easy_cleanup(pCurl);
}

bool CCurlInst::Upload(FileAttr& stFileAttr, SFTP_PROGRESS_CALLBACK_FUNC progressFunc, void* ptr)
{
    m_strSvrFileName = m_strURL + stFileAttr._strFileName;

    char progress_data[255] = { 0 };
	strcpy_s(progress_data, sizeof(progress_data), stFileAttr._strFileName.c_str());
    
    CURL* pCurl = curl_easy_init();
    curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(pCurl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
    curl_easy_setopt(pCurl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(pCurl, CURLOPT_URL, m_strSvrFileName.c_str());
    curl_easy_setopt(pCurl, CURLOPT_USERPWD, m_strUserPwd.c_str());
    curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, ReadSerFiles);
    curl_easy_setopt(pCurl, CURLOPT_READDATA, stFileAttr._fp);
    curl_easy_setopt(pCurl, CURLOPT_INFILESIZE, (long)stFileAttr._nOffset);

	curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, FALSE);
	curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, progressFunc);
    curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, progress_data);
    
//	curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 60L);
    curl_easy_setopt(pCurl, CURLOPT_FORBID_REUSE, 1);

    CURLcode res = curl_easy_perform(pCurl);
    curl_easy_cleanup (pCurl);

    if(CURLE_OK != res)
        return false;

    return true;
}

bool CCurlInst::DownLoad(FileAttr& stFileAttr, SFTP_PROGRESS_CALLBACK_FUNC progressFunc, void* ptr)
{
    m_strSvrFileName = m_strURL + stFileAttr._strFileName;

    char progress_data[255] = { 0 };
	strcpy_s(progress_data, sizeof(progress_data), stFileAttr._strFileName.c_str());

    CURL *pCurl = curl_easy_init();
    curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(pCurl, CURLOPT_URL, m_strSvrFileName.c_str());
    curl_easy_setopt(pCurl, CURLOPT_USERPWD, m_strUserPwd.c_str());
    curl_easy_setopt(pCurl, CURLOPT_TRANSFERTEXT, 0);					
	curl_easy_setopt (pCurl, CURLOPT_WRITEFUNCTION, WriteLocalData);
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA,stFileAttr._fp);
	curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, false);
	curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, progressFunc);
    curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, progress_data);
//    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 60L);
    curl_easy_setopt(pCurl, CURLOPT_FORBID_REUSE, 1);
    CURLcode res = curl_easy_perform (pCurl);
    curl_easy_cleanup (pCurl);

    if(CURLE_OK != res)
        return false;

    return true;
}

size_t CCurlInst::ReadSerFiles(void *ptr, size_t size, size_t nmemb, void *stream)
{
    curl_off_t nread;
    size_t retcode = fread(ptr, size, nmemb, (FILE*)stream);

    nread = (curl_off_t)retcode;
    return retcode;
}

size_t CCurlInst::WriteLocalData( void *ptr, size_t size, size_t nmemb, void *stream )
{
    FILE *fp = (FILE*)stream;
    size_t nFileSize = fwrite (ptr, size, nmemb, fp);

    return nFileSize;
}

// int CCurlInst::my_progress_func(char *progress_data,
// 	double t, /* dltotal */
// 	double d, /* dlnow */
// 	double ultotal,
// 	double ulnow)
// {
// 	FILE* fp;
// 	fopen_s(&fp, "progress.txt", "w+");
// 
// 	char buf[256] = { 0 };
// 	sprintf_s(buf, 256, "progress: %s %g / %g (%g %%)\n", progress_data, d, t, d * 100.0 / t);
// 	fread(buf, 256, 1, fp);
// 	fclose(fp);
// 	fp = NULL;
// 	return 0;
// }

bool CCurlInst::GetSvrFiles(std::string strServerPath, std::list<std::string>& lstFoundFiles, bool bVersion)
{
    m_lstFoundFiles.clear();

    m_strSvrFileName = m_strURL + strServerPath;

    CURL* pCurl = curl_easy_init();
    curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(pCurl, CURLOPT_URL, m_strSvrFileName.c_str());
    curl_easy_setopt(pCurl, CURLOPT_USERPWD, m_strUserPwd.c_str());
    curl_easy_setopt(pCurl, CURLOPT_DIRLISTONLY, 1);
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, ParserSvrFiles);
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &bVersion);
//    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(pCurl, CURLOPT_FORBID_REUSE, 1);
    CURLcode res = curl_easy_perform(pCurl);
    curl_easy_cleanup(pCurl);

    lstFoundFiles = m_lstFoundFiles;

    if(CURLE_OK != res)
        return false;

    return true;
}

size_t CCurlInst::ParserSvrFiles(void *ptr, size_t size, size_t nmemb, void *data)
{
    std::string::size_type  nPostPos = 0;
    std::string::size_type  nCurPos = 0;

    std::string strFileName;
    std::string strFileData = std::string((char*)ptr);

    bool bVersion = (bool)(*(char*)data);

    std::string strExt;
    if(bVersion)
    {
        strExt = ".txt";
    }
    else
    {
        strExt = ".bin";
    }

    while(1)
    {
        nPostPos = strFileData.find_first_of('\n', nCurPos);
        if(std::string::npos == nPostPos)
            break;

        strFileName.assign(strFileData, nCurPos, nPostPos - nCurPos);
        nCurPos = nPostPos + 1;


        if(std::string::npos != strFileName.find(strExt))
        {
            m_lstFoundFiles.push_back(strFileName);
        }
    }

    return nmemb;
}

