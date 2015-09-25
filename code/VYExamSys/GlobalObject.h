#pragma once
#include "..\\SQLite\\InstanceDB.h"
#include ".\\Work\DataDefine.h"

class CVYDevice;

extern CVYDevice g_oVYDevice;
extern InstanceDB g_db;		//全局唯一数据库
extern LoginInfo g_login;	//全局唯一登录信息
extern CString g_examTime;	//全局唯一考试时间
extern string g_examId;	//全局唯一考试id
