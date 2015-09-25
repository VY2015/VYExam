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
	bool		SortByOther(string exam_id,vector<string>& persons,vector<EXAM_ENTRANCE>& entrances,ROSTER_SORTKEY sortKey,bool bSort);   //��������
	bool		SortByName(string exam_id,vector<string>& persons,vector<EXAM_ROSTER>& Examroster,bool flag);	//��������
	bool		SortRosterPersons(string exam_id, vector<string>& persons,ROSTER_SORTKEY sortKey,bool flag);    //�ֶ�����
	/*
	ע�⣺
	1�����ݿ���report_type�� varchar[50] ��Ϊ int ���ͣ�
	2���볡������times�ֶΣ�������ʾ�볡������
	3���ӿ��ڲ�string��varcharֱ�ӵ����ݳ����жϡ�
	*/

	//�¼���
public:
	bool AddEventType(string version, int parent, int own, string desc);
	bool FetchChildEvent(string version, int parent/*=0*/, vector<EventNode>& nodes);
	bool ParserEvent(string version, int own_value, string desc);
	bool DelExamEvent(string version);

	////�����guidȷ��һ���¼���
	bool AddEvent(string exam_id, string guid, int report_type, EXAM_REPORT Report);
	bool UpdateEvent(string exam_id, string guid, int report_type, EXAM_REPORT Report);
	bool DelEvent(string exam_id, string guid,bool flag);//ע����Ҫ����ɾ���������¼���
	bool QueryEvent(string exam_id, vector<EXAM_REPORT>& events, bool flag/*Ĭ�ϰ�ʱ������*/);
	bool DelExamReport(string exam_id,bool flag);//ע����Ҫ����ɾ���������¼���

	////������
	bool AddAttachment(string exam_id, string guid, string path);
	bool FetchAttachment(string exam_id, string guid, vector<string>& paths);
	bool DelAttachment(string exam_id, string guid, string path);
	bool DelAllAttachments(string exam_id, string guid);
	bool DelExamAttachment(string exam_id);

	////�볡��
	bool AddEntrance(string exam_id, string regId, EXAM_ENTRANCE entrance/*�����ֶ���ɵĽṹ��*/);
	bool GetEntrances(string exam_id, vector<string/*regId*/>& regId);  //��ȡ����regId
	bool UpdateEntrance(string exam_id, string regId, EXAM_ENTRANCE entrance/*�����ֶ���ɵĽṹ��*/, int times/*�볡����Ĭ��Ϊ���һ���볡*/);
	bool DelEntranceOfPerson(string exam_id, string regId);/*ָ���˵������볡*/
	bool QueryEntranceOfPerson(string exam_id, string regId, vector<EXAM_ENTRANCE/*�����ֶ���ɵĽṹ��*/>& entrances);
	bool DelExamEntrance(string exam_id);

	////������
	bool GetExamRoomInfo(string exam_id, EXAM_ROOM& exam_room/*�����ֶ���ɵĽṹ��*/);
	bool SetExamRoomInfo(string exam_id, EXAM_ROOM exam_room);
	bool DelExamRoom(string exam_id);

	////�������
	bool AddRosterPerson(string exam_id, string regId,EXAM_ROSTER roster/*�����ֶ���ɵĽṹ��*/);
	bool DelRosterPerson(string exam_id, string regId,bool flag);//ע��������볡����Ϣ��Ҫɾ�����¼���
	int	 GetRosterPersonNum(string exam_id,int& Num);
	bool GetRosterPersons(string exam_id, vector<string/*regId*/>& persons);

	bool GetRosterPersonRegID(string exam_id,string IDNum,string& regID);

	bool GetRosterPersonInfo(string exam_id, string regId, EXAM_ROSTER& exam_roster/*�����ֶ���ɵĽṹ��*/);
	bool UpdateRosterPersonInfo(string exam_id, string regId, EXAM_ROSTER exam_roster);
	bool DelExamRoster(string exam_id,bool flag);//ע��������볡����Ϣ��Ҫɾ�����¼���

	////ǹ�ֿ��
	bool AddImposer(string version, string guid, int size, char* data);
	bool GetImposer(string version, vector<string/*guid*/>& imposer);
	bool GetImposerInfo(string version, string guid,GHOST_LIBRARY& ghost_library/*�����ֶ���ɵĽṹ��*/);
	bool DelImposer(string version, string guid);
	bool DelExamImposer(string version);

	////ȫ�����ñ�ֻ��һ�У�
	bool GetGlobalInfo(GLOBAL_CONFIG& global_config/*�����ֶ���ɵĽṹ��*/);
	bool SetGlobalInfo(GLOBAL_CONFIG global_config);

	////֤�����ͱ�
	bool AddLicence(string version, int value, string desc);
	bool GetLicence(string version, vector<LICENCE_TYPE>& vLicence_type/*�����ֶ���ɵĽṹ��*/);
	bool DelExamLicence(string version);

	////ƥ�����ñ�
	bool AddMatchConfig(string exam_id, MATCH_CONFIG match_config/*�����ֶ���ɵĽṹ��*/);
	bool UpdateMatchConfig(string exam_id, MATCH_CONFIG match_config);
	bool DelMatchConfig(string exam_id);
	bool QueryMatchConfig(string exam_id, MATCH_CONFIG& info);

	////�汾���Ʊ�
	bool AddVersionControl(string exam_id, VERSION_CONTROL version_control/*�����ֶ���ɵĽṹ��*/);
	bool UpdateVersionControl(string exam_id, VERSION_CONTROL version_control);
	bool QueryVersionControl(string exam_id, VERSION_CONTROL& version_control);
	bool DelVersionControl(string exam_id);//��ɾ������ʾ�˳����Ե����й�������Ҫɾ��
};

