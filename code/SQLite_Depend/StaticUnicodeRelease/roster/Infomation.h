#ifndef INFOMATION_H
#define INFOMATION_H
typedef struct _FreshInfo
{
	char  name[32];
	char  FreshType[32];
	char  CheckResult[32];
	int    AdmissionNO;
	char   Department[32];
	char  Major[32];
	char   CheckTime[32];
	int CheckNum;
}FreshInfo;

#endif