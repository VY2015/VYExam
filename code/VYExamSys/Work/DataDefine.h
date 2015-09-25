#pragma once


#define DYNAMIC_ENTER_NUM_MAX       9				//�볡����������(�ݲ��ɸĶ�)

typedef struct _LoginInfo{
	char examRoom[50];
	char examItem[50];
	char chiefExaminer[20];
	char assisExaminer[20];

}LoginInfo;

typedef enum _eSortType
{
	VY_SORT_CNAME,
	VY_SORT_PRESENT,
	VY_SORT_MASHINECHECK,
	VY_SORT_PERSONCHECK,
	VY_SORT_SEAT,
	VY_SORT_IMPOSTER,
	VY_SORT_HISPHOTO,
	VY_SORT_ENTRANCE
}eSortType;

typedef enum _SOCKET_CONNECT_STATUS
{
	SOCKET_RECV_SUCCESS = 0,			//���յ��ɹ�����
	SOCKET_START_ERR,					//����socketʧ��
	SOCKET_INIT_ERR,					//��ʼ��socketʧ��
	SOCKET_CONNECT_ERR,					//socket����ʧ��
	SOCKET_SEND_ERR,					//socket����ʧ��
	SOCKET_RECV_ERR,					//socket����ʧ��

	SOCKET_RECV_DATA_ERR = -1,			   //socket���յ�������Ϊ:���Բ����ڻ���δ���俼��(��½У��)
	SOCKET_RECV_DATA_EMPTY = -2,           //socket���յ�������:�������Ϊ��
	SOCKET_RECV_DATA_ID_REPEAT = -3,       //socket���յ�������:���֤���ظ�(ע�ῼ��)
	SOCKET_RECV_DATA_NO_EXAMINER = -4,     //socket���յ�������:����û�и�����(ע�ῼ��)

	SOCKET_RECV_DATA_ABNORMAL = -99,            //socket���յ�������:ϵͳ�쳣
}eSocketConnectStatus;
