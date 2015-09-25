#pragma once


#define DYNAMIC_ENTER_NUM_MAX       9				//入场次数最大个数(暂不可改动)

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
	SOCKET_RECV_SUCCESS = 0,			//接收到成功数据
	SOCKET_START_ERR,					//开启socket失败
	SOCKET_INIT_ERR,					//初始化socket失败
	SOCKET_CONNECT_ERR,					//socket连接失败
	SOCKET_SEND_ERR,					//socket发送失败
	SOCKET_RECV_ERR,					//socket接收失败

	SOCKET_RECV_DATA_ERR = -1,			   //socket接收到的数据为:考试不存在或者未分配考官(登陆校验)
	SOCKET_RECV_DATA_EMPTY = -2,           //socket接收到的数据:必填参数为空
	SOCKET_RECV_DATA_ID_REPEAT = -3,       //socket接收到的数据:身份证号重复(注册考官)
	SOCKET_RECV_DATA_NO_EXAMINER = -4,     //socket接收到的数据:考场没有负责人(注册考官)

	SOCKET_RECV_DATA_ABNORMAL = -99,            //socket接收到的数据:系统异常
}eSocketConnectStatus;
