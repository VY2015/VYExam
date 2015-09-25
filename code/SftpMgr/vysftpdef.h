#ifndef __VY_SFTP_DEF_H__
#define __VY_SFTP_DEF_H__

#include <string>

//进度条
typedef int (*SFTP_PROGRESS_CALLBACK_FUNC)(const char* flag, double dtotal, double dnow, double ultotal, double ulnow);

//上传下载状态判断
typedef enum _EVYSFTP_PROC_STATUS
{
    SFTP_CONNECT_STATUS_FIRST_START,                //第一次尝试连接
    SFTP_CONNECT_STATUS_FIRST_SUCCESS,
    SFTP_CONNECT_STATUS_FIRST_FAILURE,
    
    SFTP_CONNECT_STATUS_SECOND_START,               //第二次尝试连接
    SFTP_CONNECT_STATUS_SECOND_SUCCESS,
    SFTP_CONNECT_STATUS_SECOND_FAILURE,
    
    SFTP_DATE_VERSION_STATUS_CHECK_START,           //日期版本检测
    SFTP_DATE_VERSION_STATUS_CHECK_SUCCESS,
    SFTP_DATE_VERSION_STATUS_CHECK_FAILURE,
    
    SFTP_ROSTER_VERSION_STATUS_CHECK_START,         //花名册版本检测
    SFTP_ROSTER_VERSION_STATUS_CHECK_SUCCESS,
    SFTP_ROSTER_VERSION_STATUS_CHECK_FAILURE,
    
    SFTP_VERSION_STATUS_CHECK_NO_PASS,              //版本检测未通过，无需下载
    
    SFTP_ROSTER_STATUS_START,                       //花名册上传（下载）状态    
    SFTP_ROSTER_STATUS_SUCCESS,
    SFTP_ROSTER_STATUS_FAILURE,
    
    SFTP_IMPOSTER_STATUS_START,                     //枪手库上传（下载）状态
    SFTP_IMPOSTER_STATUS_SUCCESS,
    SFTP_IMPOSTER_STATUS_FAILURE,
    
    SFTP_HISTORY_STATUS_START,                      //历史照片下载状态
    SFTP_HISTORY_STATUS_SUCCESS,
    SFTP_HISTORY_STATUS_FAILURE,

    SFTP_SYN_STATUS_ERR,                            //err
}eVYSFTP_PROC_STATUS;


typedef enum _VYPROGRESS_STATUS
{
    PROGRESS_STATUS_SUCCESS,                        //下载成功
    PROGRESS_STATUS_FAILURE,                        //下载失败
    PROGRESS_STATUS_FORCE_IN,                       //强制进入
    PROGRESS_STATUS_LOCAL_IN,                       //本地管理
    PROGRESS_STATUS_CANCEL,                         //取消

    PROGRESS_STATUS_ERR,                            //err

}eVYPROGRESS_STATUS;

//Ftp服务器配置
typedef struct _FtpEnv
{
    std::string 		_strHostName;			//服务器名称
    std::string 		_strUserName;			//ftp用户名称
    std::string 		_strPassword;			//ftp密码
    std::string			_strPort;				//端口
    std::string 		_strFtpUploadPath;		//ftp服务器上传路径
    std::string 		_strFtpDownloadPath;	//ftp服务器下载路径
}FtpEnv;

typedef struct _LocalPath 
{
    std::string			_strLocalPath;
    std::string			_strLocalBakPath;
}LocalPath;

//Ftp配置信息
typedef struct _FtpConfigData
{
    FtpEnv          	_stFtpActive;			//FTP服务器配置信息
    LocalPath       	_stLocalPath;
}FtpConfigData;

typedef struct _FileAttr 
{
    FILE*				_fp;
	unsigned long		_nOffset;
    std::string 		_strFileName;
}FileAttr;

#endif
