#ifndef __VY_SFTP_DEF_H__
#define __VY_SFTP_DEF_H__

#include <string>

//������
typedef int (*SFTP_PROGRESS_CALLBACK_FUNC)(const char* flag, double dtotal, double dnow, double ultotal, double ulnow);

//�ϴ�����״̬�ж�
typedef enum _EVYSFTP_PROC_STATUS
{
    SFTP_CONNECT_STATUS_FIRST_START,                //��һ�γ�������
    SFTP_CONNECT_STATUS_FIRST_SUCCESS,
    SFTP_CONNECT_STATUS_FIRST_FAILURE,
    
    SFTP_CONNECT_STATUS_SECOND_START,               //�ڶ��γ�������
    SFTP_CONNECT_STATUS_SECOND_SUCCESS,
    SFTP_CONNECT_STATUS_SECOND_FAILURE,
    
    SFTP_DATE_VERSION_STATUS_CHECK_START,           //���ڰ汾���
    SFTP_DATE_VERSION_STATUS_CHECK_SUCCESS,
    SFTP_DATE_VERSION_STATUS_CHECK_FAILURE,
    
    SFTP_ROSTER_VERSION_STATUS_CHECK_START,         //������汾���
    SFTP_ROSTER_VERSION_STATUS_CHECK_SUCCESS,
    SFTP_ROSTER_VERSION_STATUS_CHECK_FAILURE,
    
    SFTP_VERSION_STATUS_CHECK_NO_PASS,              //�汾���δͨ������������
    
    SFTP_ROSTER_STATUS_START,                       //�������ϴ������أ�״̬    
    SFTP_ROSTER_STATUS_SUCCESS,
    SFTP_ROSTER_STATUS_FAILURE,
    
    SFTP_IMPOSTER_STATUS_START,                     //ǹ�ֿ��ϴ������أ�״̬
    SFTP_IMPOSTER_STATUS_SUCCESS,
    SFTP_IMPOSTER_STATUS_FAILURE,
    
    SFTP_HISTORY_STATUS_START,                      //��ʷ��Ƭ����״̬
    SFTP_HISTORY_STATUS_SUCCESS,
    SFTP_HISTORY_STATUS_FAILURE,

    SFTP_SYN_STATUS_ERR,                            //err
}eVYSFTP_PROC_STATUS;


typedef enum _VYPROGRESS_STATUS
{
    PROGRESS_STATUS_SUCCESS,                        //���سɹ�
    PROGRESS_STATUS_FAILURE,                        //����ʧ��
    PROGRESS_STATUS_FORCE_IN,                       //ǿ�ƽ���
    PROGRESS_STATUS_LOCAL_IN,                       //���ع���
    PROGRESS_STATUS_CANCEL,                         //ȡ��

    PROGRESS_STATUS_ERR,                            //err

}eVYPROGRESS_STATUS;

//Ftp����������
typedef struct _FtpEnv
{
    std::string 		_strHostName;			//����������
    std::string 		_strUserName;			//ftp�û�����
    std::string 		_strPassword;			//ftp����
    std::string			_strPort;				//�˿�
    std::string 		_strFtpUploadPath;		//ftp�������ϴ�·��
    std::string 		_strFtpDownloadPath;	//ftp����������·��
}FtpEnv;

typedef struct _LocalPath 
{
    std::string			_strLocalPath;
    std::string			_strLocalBakPath;
}LocalPath;

//Ftp������Ϣ
typedef struct _FtpConfigData
{
    FtpEnv          	_stFtpActive;			//FTP������������Ϣ
    LocalPath       	_stLocalPath;
}FtpConfigData;

typedef struct _FileAttr 
{
    FILE*				_fp;
	unsigned long		_nOffset;
    std::string 		_strFileName;
}FileAttr;

#endif
