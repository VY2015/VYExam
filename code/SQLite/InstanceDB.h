#include <string>
#include <vector>


#ifndef __VY_EXAM_FLIE_DEF_H__
#include "vyexamfiledef.h"
#endif

using namespace std;

class _declspec(dllexport) InstanceDB
{
public:
	InstanceDB(string dbstr);
	InstanceDB(const wchar_t* dbstr);
	~InstanceDB();

	void BeginTransaction();
	void CommitTransaction();
	void RollbackTransaction();

	int execute(string str);
	int execute(const wchar_t* str);


private:
	std::string			Sort(ROSTER_SORTKEY sortKey,bool flag=true);
	bool				GetRosterPersonName(string exam_id,vector<EXAM_ROSTER>& Examroster);

public:
	bool		SortByOther(string exam_id,vector<string>& persons,vector<EXAM_ENTRANCE>& entrances,ROSTER_SORTKEY sortKey,bool bSort);   //核验排序
	bool		SortByName(string exam_id,vector<string>& persons,vector<EXAM_ROSTER>& Examroster,bool flag);	//姓名排序
	bool		SortRosterPersons(string exam_id, vector<string>& persons,ROSTER_SORTKEY sortKey,bool flag);    //字段排序
	/*
	注意：
	1、数据库中report_type由 varchar[50] 改为 int 类型；
	2、入场表增加times字段，用来表示入场次数；
	3、接口内部string与varchar直接的数据长度判断。
	*/

	//事件表
public:
	bool AddEventType(string version, int parent, int own, string desc);
	bool FetchChildEvent(string version, int parent/*=0*/, vector<EventNode>& nodes);
	bool ParserEvent(string version, int own_value, string desc);
	bool DelExamEvent(string version);

	////报告表（guid确定一个事件）
	bool AddEvent(string exam_id, string guid, int report_type, EXAM_REPORT Report);
	bool UpdateEvent(string exam_id, string guid, int report_type, EXAM_REPORT Report);
	bool DelEvent(string exam_id, string guid,bool flag);//注意需要联表删除附件（事件）
	bool QueryEvent(string exam_id, vector<EXAM_REPORT>& events, bool flag/*默认按时间正序*/);
	bool DelExamReport(string exam_id,bool flag);//注意需要联表删除附件（事件）

	////附件表
	bool AddAttachment(string exam_id, string guid, string path);
	bool FetchAttachment(string exam_id, string guid, vector<string>& paths);
	bool DelAttachment(string exam_id, string guid, string path);
	bool DelAllAttachments(string exam_id, string guid);
	bool DelExamAttachment(string exam_id);

	////入场表
	bool AddEntrance(string exam_id, string regId, EXAM_ENTRANCE entrance/*所有字段组成的结构体*/);
	bool GetEntrances(string exam_id, vector<string/*regId*/>& regId);  //获取所有regId
	bool UpdateEntrance(string exam_id, string regId, EXAM_ENTRANCE entrance/*所有字段组成的结构体*/, int times/*入场次序，默认为最后一次入场*/);
	bool DelEntranceOfPerson(string exam_id, string regId);/*指此人的所有入场*/
	bool QueryEntranceOfPerson(string exam_id, string regId, vector<EXAM_ENTRANCE/*所有字段组成的结构体*/>& entrances);
	bool DelExamEntrance(string exam_id);

	////考场表
	bool GetExamRoomInfo(string exam_id, EXAM_ROOM& exam_room/*所有字段组成的结构体*/);
	bool SetExamRoomInfo(string exam_id, EXAM_ROOM exam_room);
	bool DelExamRoom(string exam_id);

	////花名册表
	bool AddRosterPerson(string exam_id, string regId,EXAM_ROSTER roster/*所有字段组成的结构体*/);
	bool DelRosterPerson(string exam_id, string regId,bool flag);//注意关联的入场表信息需要删除（事件）
	int	 GetRosterPersonNum(string exam_id,int& Num);
	bool GetRosterPersons(string exam_id, vector<string/*regId*/>& persons);

	bool GetRosterPersonRegID(string exam_id,string IDNum,string& regID);

	bool GetRosterPersonInfo(string exam_id, string regId, EXAM_ROSTER& exam_roster/*所有字段组成的结构体*/);
	bool UpdateRosterPersonInfo(string exam_id, string regId, EXAM_ROSTER exam_roster);
	bool DelExamRoster(string exam_id,bool flag);//注意关联的入场表信息需要删除（事件）

	////枪手库表
	bool AddImposer(string version, string guid, int size, char* data);
	bool GetImposer(string version, vector<string/*guid*/>& imposer);
	bool GetImposerInfo(string version, string guid,GHOST_LIBRARY& ghost_library/*所有字段组成的结构体*/);
	bool DelImposer(string version, string guid);
	bool DelExamImposer(string version);

	////全局配置表（只有一行）
	bool GetGlobalInfo(GLOBAL_CONFIG& global_config/*所有字段组成的结构体*/);
	bool SetGlobalInfo(GLOBAL_CONFIG global_config);

	////证件类型表
	bool AddLicence(string version, int value, string desc);
	bool GetLicence(string version, vector<LICENCE_TYPE>& vLicence_type/*所有字段组成的结构体*/);
	bool DelExamLicence(string version);

	////匹配配置表
	bool AddMatchConfig(string exam_id, MATCH_CONFIG match_config/*所有字段组成的结构体*/);
	bool UpdateMatchConfig(string exam_id, MATCH_CONFIG match_config);
	bool DelMatchConfig(string exam_id);
	bool QueryMatchConfig(string exam_id, MATCH_CONFIG& info);

	////版本控制表
	bool AddVersionControl(string exam_id, VERSION_CONTROL version_control/*所有字段组成的结构体*/);
	bool UpdateVersionControl(string exam_id, VERSION_CONTROL version_control);
	bool QueryVersionControl(string exam_id, VERSION_CONTROL& version_control);
	bool DelVersionControl(string exam_id);//此删除，表示此场考试的所有关联表都需要删除
};

