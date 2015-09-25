#include "InstanceDB.h"
#include "wx/wxsqlite3.h"
#include "wx/string.h"
#include <iostream>
#include <comdef.h>
#include "LangSort.h"

using namespace std;

static wxSQLite3Database *db;

static wxString stringTowxString(string s)
{
	const char* _Source = s.c_str();
	size_t _Dsize = MultiByteToWideChar(CP_ACP, 0, _Source, -1, NULL, 0);
	wchar_t *_Dest = new wchar_t[_Dsize];
	MultiByteToWideChar(CP_ACP, 0, _Source, -1, _Dest, _Dsize);
	wstring result = _Dest;
	delete []_Dest;
	return result;
}

// wxString stringTowxString(string str)
// {
// 	return wxString(_bstr_t(str.c_str()));
// }

InstanceDB::InstanceDB(string dbstr)
{
	db = NULL;
	db = new wxSQLite3Database();
	if(!db)
		return;
	db->Open(stringTowxString(dbstr));
}

InstanceDB::InstanceDB(const wchar_t* dbstr)
{
	db = NULL;
	db = new wxSQLite3Database();
	if(!db)
		return;
	db->Open(dbstr);
}

InstanceDB::~InstanceDB()
{
	if(!db)
		return;
	db->Close();
	db = NULL;
	delete db;
}

void InstanceDB::BeginTransaction()
{
	db->Begin();
}

void InstanceDB::CommitTransaction()
{
	db->Commit();
}

void InstanceDB::RollbackTransaction()
{
	db->Rollback();
}

int InstanceDB::execute(string str)
{
	int a = db->ExecuteUpdate(stringTowxString(str));
	return a;
}

int InstanceDB::execute(const wchar_t* str)
{
	return db->ExecuteUpdate(str);
}

bool InstanceDB::AddEventType(string version, int parent, int own, string desc)
{
	char cmd[1024];
	sprintf(cmd, "INSERT INTO T_EVENT_TYPE VALUES ('%s',%d,%d,'%s')", version.data(), parent, own, desc.data());
	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::FetchChildEvent(string version, int parent, vector<EventNode>& nodes)
{
	char cmd[1024];
	sprintf(cmd, "SELECT * FROM T_EVENT_TYPE WHERE event_type_version = '%s' AND parent_value = %d", version.data(), parent);

	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		nodes.push_back(EventNode());
		string str;
		nodes.back().Own_value = set.GetInt(L"own_value");
		str = set.GetAsString(L"discription").mb_str();
		str.copy(nodes.back().Dsec, 50, 0);
	}

// 	if(nodes.size() == 0)
// 		return false;
	
	return true;
}

bool InstanceDB::ParserEvent(string version, int own_value, string desc)
{
	char cmd[1024];
	sprintf(cmd, "SELECT * FROM T_EVENT_TYPE WHERE event_type_version = '%s' AND own_value = %d", version.data(), own_value);
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		string str = set.GetAsString(L"discription").mb_str();
		desc = str;
	}

	return true;
}

bool InstanceDB::DelExamEvent(string version)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_EVENT_TYPE WHERE event_type_version = '%s'",version.data());
	if(execute(cmd))
		return true;
	return false;
}


//附件表
bool InstanceDB::AddAttachment(string exam_id, string guid, string path)
{
	char cmd[1024];
	sprintf(cmd, "INSERT INTO T_EXAM_ATTACHMENT VALUES ('%s','%s','%s')", exam_id.data(),guid.data(),path.data());
	if(execute(cmd))
		return true;
	return false;

}

bool InstanceDB::FetchAttachment(string exam_id, string guid, vector<string>& paths)
{
	char cmd[1024];
	sprintf(cmd,"SELECT * FROM T_EXAM_ATTACHMENT WHERE exam_id='%s'AND attachment_guid='%s' ",exam_id.data(),guid.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		string str;
		str = set.GetAsString(L"attachment_path").mb_str();
		paths.push_back(str);

	}

// 	if(paths.size() == 0)
// 		return false;

	return true;

}

bool InstanceDB::DelAttachment(string exam_id, string guid, string path)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_EXAM_ATTACHMENT WHERE exam_id='%s'AND attachment_guid='%s' AND attachment_path='%s'",exam_id.data(),guid.data(),path.data());
	if(execute(cmd))
		return true;
	return false;

}


bool InstanceDB::DelAllAttachments(string exam_id, string guid)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_EXAM_ATTACHMENT WHERE exam_id='%s'AND attachment_guid='%s'",exam_id.data(),guid.data());
	if(execute(cmd))
		return true;
	return false;

}


bool InstanceDB::DelExamAttachment(string exam_id)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_EXAM_ATTACHMENT WHERE exam_id='%s'",exam_id.data());
	if(execute(cmd))
		return true;
	return false;

}


//报告表
bool InstanceDB::AddEvent(string exam_id, string guid, int report_type, EXAM_REPORT Report)
{
	char cmd[1024];
	sprintf(cmd,"INSERT INTO T_EXAM_REPORT VALUES ('%s',%d,'%s','%s','%s','%s')",
		exam_id.data(),report_type,Report.Occurrence_time,Report.People,Report.Detail,guid.data());

	if(execute(cmd))
		return true;
	return false;
}


bool InstanceDB::UpdateEvent(string exam_id, string guid, int report_type, EXAM_REPORT Report)
{
	char cmd[1024];
	sprintf(cmd,"UPDATE T_EXAM_REPORT set occurrence_time='%s', people='%s' , detail='%s' , \
				 WHERE exam_id='%s' AND attachment_guid='%s' AND report_type=%d",
				 Report.Occurrence_time,Report.People,Report.Detail,exam_id.data(),guid.data(),report_type);
	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::DelEvent(string exam_id, string guid,bool flag)
{

	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_EXAM_REPORT WHERE exam_id='%s'AND attachment_guid='%s'",exam_id.data(),guid.data());
	
	if(flag)
	{
		BeginTransaction();
		if(execute(cmd))
		{
			if(DelAllAttachments(exam_id,guid))
			{
				CommitTransaction();
				return true;
			}
		}
		else
			RollbackTransaction();	
	}
	else
	{
		if(execute(cmd))
			return true;
	}
	return false;
}

bool InstanceDB::QueryEvent(string exam_id,vector<EXAM_REPORT>& events, bool flag)
{
	char cmd[1024];
	EXAM_REPORT exam_report;
	wxSQLite3ResultSet set;

	if(flag)
	{
		sprintf(cmd, "SELECT * FROM T_EXAM_REPORT WHERE exam_id = '%s' ORDER BY occurrence_time  ASC ", exam_id.data());
		set = db->ExecuteQuery(cmd);

		while(set.NextRow())
		{
			exam_report.Report_type = set.GetInt(L"report_type");
			strcpy(exam_report.Occurrence_time,set.GetAsString(L"occurrence_time").mb_str());

			//exam_report.People = set.GetAsString(L"people");
			//exam_report.Detail = set.GetAsString(L"detail");

			string people = set.GetAsString(L"people").mb_str();
			exam_report.People = people;
			string detail = set.GetAsString(L"detail").mb_str();
			exam_report.Detail = detail;

			strcpy(exam_report.Attachment_guid,set.GetAsString(L"attachment_guid").mb_str());

			events.push_back(exam_report);
		}

// 		if(events.size() == 0)
// 			return false;
	}
	else
	{
		sprintf(cmd, "SELECT * FROM T_EXAM_REPORT WHERE exam_id = '%s' ORDER BY occurrence_time  DESC ", exam_id.data());
		set = db->ExecuteQuery(cmd);

		while(set.NextRow())
		{
			exam_report.Report_type = set.GetInt(L"report_type");
			strcpy(exam_report.Occurrence_time,set.GetAsString(L"occurrence_time").mb_str());

			//exam_report.People = set.GetAsString(L"people");
			//exam_report.Detail = set.GetAsString(L"detail");

			string people = set.GetAsString(L"people").mb_str();
			exam_report.People = people;
			string detail = set.GetAsString(L"detail").mb_str();
			exam_report.Detail = detail;

			strcpy(exam_report.Attachment_guid,set.GetAsString(L"attachment_guid").mb_str());

			events.push_back(exam_report);
		}

// 		if(events.size() == 0)
// 			return false;
	}
	
	return true;
}


bool InstanceDB::DelExamReport(string exam_id,bool flag)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_EXAM_REPORT WHERE exam_id='%s'",exam_id.data());

	if(flag)
	{
		BeginTransaction();
		if(execute(cmd))
		{
			if(DelExamAttachment(exam_id))
			{
				CommitTransaction();
				return true;
			}
		}
		else
			RollbackTransaction();
	}
	else
	{
		if(execute(cmd))
			return true;
	}
	return false;
}





//入场表
bool InstanceDB::AddEntrance(string exam_id, string regId, EXAM_ENTRANCE entrance/*所有字段组成的结构体*/)
{
	char cmd[1024];
	sprintf(cmd,"INSERT INTO T_EXAM_ENTRANCE VALUES ('%s','%s','%s','%s',%d,%d,%d,%d,%d)"
		,exam_id.data(),regId.data(),entrance.Entrance_time,entrance.Entrance_photo_path,
		entrance.Ghost_check,entrance.Machine_check,entrance.Man_check,entrance.History_check,entrance.Times);

	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::GetEntrances(string exam_id, vector<string>& regId)
{
	char cmd[1024];
	char Regid[50];
	sprintf(cmd, "SELECT * FROM T_EXAM_ENTRANCE WHERE exam_id = '%s'", exam_id.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		strcpy(Regid,set.GetAsString(L"fix_register_id").mb_str());
		regId.push_back(Regid);
	}

//	if(regId.size() == 0)
//		return false;

	return true;
}

bool InstanceDB::UpdateEntrance(string exam_id, string regId, EXAM_ENTRANCE entrance, int times/*入场次序，默认为最后一次入场*/)
{
	char cmd[1024];
	sprintf(cmd,"UPDATE T_EXAM_ENTRANCE set entrance_time='%s', entrance_photo_path='%s' , ghost_check=%d , \
	 machine_check=%d , man_check=%d , history_check=%d WHERE exam_id='%s' AND fix_register_id='%s' AND times=%d",
		 entrance.Entrance_time,entrance.Entrance_photo_path,entrance.Ghost_check,entrance.Machine_check,entrance.Man_check,entrance.History_check,
		 exam_id.data(),regId.data(),times);
	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::DelEntranceOfPerson(string exam_id, string regId)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_EXAM_ENTRANCE WHERE exam_id='%s'AND fix_register_id='%s'",exam_id.data(),regId.data());
	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::QueryEntranceOfPerson(string exam_id, string regId, vector<EXAM_ENTRANCE>& entrances)
{

	char cmd[1024];
	sprintf(cmd, "SELECT * FROM T_EXAM_ENTRANCE WHERE exam_id = '%s' AND fix_register_id = '%s'", exam_id.data(), regId.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		//EXAM_ENTRANCE exam_entrance;
		entrances.push_back(EXAM_ENTRANCE());
		string strTime,strPath;

		strTime = set.GetAsString(L"entrance_time").mb_str();
		strTime.copy(entrances.back().Entrance_time, 50, 0);

		strPath = set.GetAsString(L"entrance_photo_path").mb_str();
		strPath.copy(entrances.back().Entrance_photo_path, 50, 0);

		entrances.back().Ghost_check = set.GetInt(L"ghost_check");
		entrances.back().Machine_check = set.GetInt(L"machine_check");
		entrances.back().Man_check = set.GetInt(L"man_check");
		entrances.back().History_check = set.GetInt(L"history_check");
		entrances.back().Times = set.GetInt(L"times");
	}
// 	if(entrances.size() == 0)
// 		return false;

	return true;
}

bool InstanceDB::DelExamEntrance(string exam_id)
{

	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_EXAM_ENTRANCE WHERE exam_id='%s'",exam_id.data());
	if(execute(cmd))
		return true;
	return false;
}


//考场表
bool InstanceDB::GetExamRoomInfo(string exam_id, EXAM_ROOM& exam_room/*所有字段组成的结构体*/)
{
	char cmd[1024];
	sprintf(cmd, "SELECT * FROM T_EXAM_ROOM WHERE exam_id = '%s'", exam_id.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		strcpy(exam_room.Chief_examiner_id,set.GetAsString(L"chief_examiner_id").mb_str());
		strcpy(exam_room.Chief_examiner_name,set.GetAsString(L"chief_examiner_name").mb_str());
		strcpy(exam_room.Assistant_examiner_id,set.GetAsString(L"assistant_examiner_id").mb_str());
		strcpy(exam_room.Assistant_examiner_name,set.GetAsString(L"assistant_examiner_name").mb_str());
	}
	return true;
}


bool InstanceDB::SetExamRoomInfo(string exam_id, EXAM_ROOM exam_room)
{
	char cmd[1024];
	sprintf(cmd,"UPDATE T_EXAM_ROOM set chief_examiner_id='%s', chief_examiner_name='%s',assistant_examiner_id = '%s',assistant_examiner_name = '%s'WHERE exam_id='%s'",
			exam_room.Chief_examiner_id,exam_room.Chief_examiner_name,exam_room.Assistant_examiner_id,exam_room.Assistant_examiner_name,exam_id.data());
	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::DelExamRoom(string exam_id)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_EXAM_ROOM WHERE exam_id='%s'",exam_id.data());
	if(execute(cmd))
		return true;
	return false;

}

//花名册表
bool InstanceDB::AddRosterPerson(string exam_id, string regId,EXAM_ROSTER roster)
{
	char cmd[2048];
	sprintf(cmd,"INSERT INTO T_EXAM_ROSTER VALUES ('%s','%s',%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s', \
		'%s','%s','%s','%s','%s',%d,'%s','%s','%s','%s',%d)",
		exam_id.data(),regId.data(),roster.Fix_licence_type,roster.Fix_chs_surname,roster.Fix_chs_name,
		roster.Fix_en_surname,roster.Fix_en_name,roster.Fix_nation,roster.Fix_sex,roster.Fix_born,
		roster.Fix_addr,roster.Fix_card_num,roster.Fix_police,roster.Fix_active,roster.Full_name,
		roster.Nation,roster.Sex,roster.Born,roster.Addr,roster.Card_num,roster.Police,roster.Active,
		roster.Attend,roster.Seat,roster.Licence_photo_path,roster.Head_photo_path,roster.Fix_history_photo_path,roster.Times);
	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::DelRosterPerson(string exam_id, string regId,bool flag)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_EXAM_ROSTER WHERE exam_id='%s'AND fix_register_id='%s'",exam_id.data(),regId.data());
	
	if(flag)
	{
		BeginTransaction();
		if(execute(cmd))
		{
			if(DelEntranceOfPerson(exam_id,regId))
			{
				CommitTransaction();
				return true;
			}
		}
		else
			RollbackTransaction();
	}
	else
	{
		if(execute(cmd))
			return true;
	}
	return false;
}

int InstanceDB::GetRosterPersonNum(string exam_id,int& Num)
{
	char cmd[1024];
	sprintf(cmd, "SELECT COUNT(*) FROM T_EXAM_ROSTER WHERE exam_id = '%s'", exam_id.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	Num = set.GetColumnCount();
	return Num;
}

bool InstanceDB::GetRosterPersons(string exam_id, vector<string/*regId*/>& persons)
{
	char cmd[1024];
	char Regid[50];
	sprintf(cmd, "SELECT * FROM T_EXAM_ROSTER WHERE exam_id = '%s'", exam_id.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		strcpy(Regid,set.GetAsString(L"fix_register_id").mb_str());
		persons.push_back(Regid);
	}
// 	if(persons.size() == 0)
// 		return false;

	return true;
}

bool InstanceDB::GetRosterPersonName(string exam_id,vector<EXAM_ROSTER>& Examroster)
{
	char cmd[1024];
	char Regid[50];
	EXAM_ROSTER exam;

	sprintf(cmd, "SELECT * FROM T_EXAM_ROSTER WHERE exam_id = '%s'", exam_id.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		strcpy(exam.Full_name,set.GetAsString(L"full_name").mb_str());
		strcpy(exam.Fix_register_id,set.GetAsString(L"fix_register_id").mb_str());
		Examroster.push_back(exam);

// 		strcpy(Regid,set.GetAsString(L"fix_register_id").mb_str());
// 		persons.push_back(Regid);
	}
// 	if(Examroster.size() == 0)
// 		return false;

	return true;
}

//////////////////////////////
bool InstanceDB::GetRosterPersonRegID(string exam_id,string IDNum,string& regID)
{
	char cmd[1024];
	sprintf(cmd, "SELECT fix_register_id FROM T_EXAM_ROSTER WHERE exam_id = '%s' AND fix_card_num='%s' ", exam_id.data(),IDNum.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		string ID = set.GetAsString(L"fix_register_id").mb_str();
		regID = ID;
	}
	return true;
}

bool InstanceDB::SortRosterPersons(string exam_id, vector<string/*regId*/>& persons,ROSTER_SORTKEY sortKey,bool flag)
{
	char Regid[50];
	std::string cmd= Sort(sortKey,flag);
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd.c_str());
	while(set.NextRow())
	{
		strcpy(Regid,set.GetAsString(L"fix_register_id").mb_str());
		persons.push_back(Regid);
	}
	return true;

}
bool InstanceDB::GetRosterPersonInfo(string exam_id, string regId, EXAM_ROSTER& exam_roster)
{
	char cmd[1024];
	sprintf(cmd, "SELECT * FROM T_EXAM_ROSTER WHERE exam_id = '%s'AND fix_register_id='%s'", exam_id.data(),regId.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		exam_roster.Fix_licence_type = set.GetInt(L"fix_licence_type");
		strcpy(exam_roster.Exam_id,set.GetAsString(L"exam_id").mb_str());
		strcpy(exam_roster.Fix_register_id,set.GetAsString(L"fix_register_id").mb_str());
		strcpy(exam_roster.Fix_chs_surname,set.GetAsString(L"fix_chs_surname").mb_str());
		strcpy(exam_roster.Fix_chs_name,set.GetAsString(L"fix_chs_name").mb_str());
		strcpy(exam_roster.Fix_en_surname,set.GetAsString(L"fix_en_surname").mb_str());
		strcpy(exam_roster.Fix_en_name,set.GetAsString(L"fix_en_name").mb_str());
		strcpy(exam_roster.Fix_nation,set.GetAsString(L"fix_nation").mb_str());
		strcpy(exam_roster.Fix_sex,set.GetAsString(L"fix_sex").mb_str());
		strcpy(exam_roster.Fix_born,set.GetAsString(L"fix_born").mb_str());
		strcpy(exam_roster.Fix_addr,set.GetAsString(L"fix_addr").mb_str());
		strcpy(exam_roster.Fix_card_num,set.GetAsString(L"fix_card_num").mb_str());
		strcpy(exam_roster.Fix_police,set.GetAsString(L"fix_police").mb_str());
		strcpy(exam_roster.Fix_active,set.GetAsString(L"fix_active").mb_str());
		strcpy(exam_roster.Full_name,set.GetAsString(L"full_name").mb_str());
		strcpy(exam_roster.Nation,set.GetAsString(L"nation").mb_str());
		strcpy(exam_roster.Sex,set.GetAsString(L"sex").mb_str());
		strcpy(exam_roster.Born,set.GetAsString(L"born").mb_str());
		strcpy(exam_roster.Addr,set.GetAsString(L"addr").mb_str());
		strcpy(exam_roster.Card_num,set.GetAsString(L"card_num").mb_str());
		strcpy(exam_roster.Police,set.GetAsString(L"police").mb_str());
		strcpy(exam_roster.Active,set.GetAsString(L"active").mb_str());
		
		exam_roster.Attend = set.GetInt(L"attend");
		strcpy(exam_roster.Seat,set.GetAsString(L"seat").mb_str());
		strcpy(exam_roster.Licence_photo_path,set.GetAsString(L"licence_photo_path").mb_str());
		strcpy(exam_roster.Head_photo_path,set.GetAsString(L"head_photo_path").mb_str());
		strcpy(exam_roster.Fix_history_photo_path,set.GetAsString(L"fix_history_photo_path").mb_str());

		exam_roster.Times = set.GetInt(L"times");

	}

	set.Finalize();

	return true;
}


bool InstanceDB::UpdateRosterPersonInfo(string exam_id, string regId, EXAM_ROSTER exam_roster)
{
	char cmd[1024];
	sprintf(cmd,"UPDATE T_EXAM_ROSTER set fix_licence_type=%d, fix_chs_surname='%s',fix_chs_name = '%s',fix_en_surname = '%s',fix_en_name = '%s', \
		fix_nation = '%s',fix_sex = '%s',fix_born = '%s',fix_addr = '%s',fix_card_num = '%s',fix_police = '%s',fix_active = '%s',full_name = '%s',\
		nation = '%s',sex = '%s',born = '%s',addr = '%s',card_num = '%s',police = '%s',active = '%s',attend = %d,seat = '%s',licence_photo_path = '%s',\
		head_photo_path = '%s',fix_history_photo_path = '%s',times=%d WHERE exam_id='%s'AND fix_register_id='%s'",
		exam_roster.Fix_licence_type,exam_roster.Fix_chs_surname,exam_roster.Fix_chs_name,exam_roster.Fix_en_surname,exam_roster.Fix_en_name,
		exam_roster.Fix_nation,exam_roster.Fix_sex,exam_roster.Fix_born,exam_roster.Fix_addr,exam_roster.Fix_card_num,exam_roster.Fix_police,
		exam_roster.Fix_active,exam_roster.Full_name,exam_roster.Nation,exam_roster.Sex,exam_roster.Born,exam_roster.Addr,exam_roster.Card_num,
		exam_roster.Police,exam_roster.Active,exam_roster.Attend,exam_roster.Seat,exam_roster.Licence_photo_path,exam_roster.Head_photo_path,
		exam_roster.Fix_history_photo_path,exam_roster.Times,exam_id.data(),regId.data());

	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::DelExamRoster(string exam_id,bool flag)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_EXAM_ROSTER WHERE exam_id='%s'",exam_id.data());

	if(flag)
	{
		BeginTransaction();
		if(execute(cmd))
		{
			if(DelExamEntrance(exam_id))
			{
				CommitTransaction();
				return true;
			}
		}
		else
			RollbackTransaction();
	}
	else
	{
		if(execute(cmd))
			return true;
	}
	return false;
}

std::string InstanceDB::Sort(ROSTER_SORTKEY sortKey,bool flag)
{
	string cmd = "SELECT * FROM T_EXAM_ROSTER ORDER BY";
	string ASC = "ASC";
	string DESC = "DESC";
	//collate pinyin
	switch(sortKey)
	{
	case VY_SORT_NAME:
		if(flag)
			cmd += " full_name "+ ASC;
		else
			cmd += " full_name "+ DESC;
		break;
	case VY_SORT_ATTEND:
		if(flag)
			cmd += " attend " + ASC;
		else
			cmd += " attend " + DESC;
		break;
	case VY_SORT_REGISTER_ID:
		if(flag)
			cmd += " fix_register_id " + ASC;
		else
			cmd += " fix_register_id " + DESC;
		break;
	case VY_SORT_SEATS:
		if(flag)
			cmd += " seat " + ASC;
		else
			cmd += " seat " + DESC;
		break;
	case VY_SORT_TIMES:
		if(flag)
			cmd += " times " + ASC;
		else
			cmd += " times " + DESC;
		break;
	default:
		MessageBoxA(NULL, "排序字段选择错误", "提示", MB_OK);
	}

	return cmd;

}


bool InstanceDB::SortByOther(string exam_id,vector<string>& persons,vector<EXAM_ENTRANCE>& entrances,ROSTER_SORTKEY sortKey,bool bSort)
{
	 
	if(!GetEntrances(exam_id, persons))
		return false;

	char cmd[1024];
	char reg[64] = {};
	EXAM_ENTRANCE m_entrance;

	const char* temp = new char[256];
	temp = persons.front().data();
	int It = 0;
	for(vector<string>::iterator iter = persons.begin();iter != persons.end();iter++)
	{

		if(strcmp(temp,(*iter).data()) == 0 && It != 0)
		{
			continue;
		}
		It ++;
		temp = (*iter).data();

		sprintf(cmd,"Select * From T_EXAM_ENTRANCE  where fix_register_id='%s' order by times DESC limit 0,1",(*iter).data());
		wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
		while(set.NextRow())
		{
			strcpy(m_entrance.Fix_register_id ,set.GetAsString(L"fix_register_id").mb_str());
			m_entrance.Man_check = set.GetInt(L"man_check");
			m_entrance.Ghost_check = set.GetInt(L"ghost_check");
			m_entrance.History_check = set.GetInt(L"history_check");
			m_entrance.Times = set.GetInt(L"times");
			entrances.push_back(m_entrance);

		}

	}

	unsigned int uSize  = entrances.size();
	char(*str)[64] = new char[uSize][64];
	unsigned int uIndex = 0;
	char buf[64] = {};

	for(vector<EXAM_ENTRANCE>::iterator iter = entrances.begin();iter != entrances.end();iter++)
	{
		switch(sortKey)
		{
		case VY_SORT_NAME:
		//	strcpy(buf,"23");
			break;
		case VY_SORT_MANCHECK:

			if((*iter).Times > 0)
				itoa((*iter).Man_check,buf,10);
			else
				strcpy(buf,"-1");
			break;
		case VY_SORT_IMPOSTERS:
			if((*iter).Times > 0)
				itoa((*iter).Ghost_check,buf,10);
			else
				strcpy(buf,"-1");
			break;
		case VY_SORT_HISPHOTOS:
			if((*iter).Times > 0)
				itoa((*iter).History_check,buf,10);
			else
				strcpy(buf,"-1");
			break;
		default:
			//MessageBoxA(NULL,"排序字段选择错误","提示",MB_OK);
			persons.clear();
			return false;
		}
		strcpy_s((char*)(str + uIndex++), 64, buf);
	}

	VY_ChineseSort(uSize,str);
	
	persons.clear();
	std::vector<EXAM_ENTRANCE> m_vecentrance = entrances;
	for(int i = 0 ; i < uSize ; i++)
	{
		for(std::vector<EXAM_ENTRANCE>::iterator iter = m_vecentrance.begin();iter != m_vecentrance.end();)
		{
			memset(buf, 0, sizeof(buf));
			switch(sortKey)
			{
			case VY_SORT_NAME:
				//	strcpy(buf,"23");
				break;
			case VY_SORT_MANCHECK:

				if((*iter).Times > 0)
					itoa((*iter).Man_check,buf,10);
				else
					strcpy(buf,"-1");
				break;
			case VY_SORT_IMPOSTERS:
				if((*iter).Times > 0)
					itoa((*iter).Ghost_check,buf,10);
				else
					strcpy(buf,"-1");
				break;
			case VY_SORT_HISPHOTOS:
				if((*iter).Times > 0)
					itoa((*iter).History_check,buf,10);
				else
					strcpy(buf,"-1");
				break;
			default:
				//MessageBoxA(NULL,"排序字段选择错误","提示",MB_OK);
				persons.clear();
				return false;
			}
 			
			if(strcmp(str[i],buf) == 0)
			{
				persons.push_back((*iter).Fix_register_id);
				m_vecentrance.erase(iter);
				break;
			}
			iter ++;
		}
	}

	delete[] str;
	
	if (!bSort)
	{
		reverse(persons.begin(), persons.end());
	}
	return true;
}

bool InstanceDB::SortByName(string exam_id,vector<string/*regId*/>& persons,vector<EXAM_ROSTER>& Examroster,bool flag)
{
	if(!GetRosterPersonName(exam_id,Examroster))
		return false;

	unsigned int uSize  = Examroster.size();
	char(*str)[64] = new char[uSize][64];
	unsigned int uIndex = 0;
	char buf[64] = {};

	for(vector<EXAM_ROSTER>::iterator iter = Examroster.begin();iter != Examroster.end();iter++)
	{
		strcpy(buf,(*iter).Full_name);
		strcpy_s((char*)(str + uIndex++), 64, buf);
	}

	VY_ChineseSort(uSize,str);
	persons.clear();

	std::vector<EXAM_ROSTER> m_vecexamroster = Examroster;
	for(int i = 0 ; i < uSize ; i++)
	{
		for(std::vector<EXAM_ROSTER>::iterator iter = m_vecexamroster.begin();iter != m_vecexamroster.end();)
		{
			memset(buf, 0, sizeof(buf));
			strcpy(buf,(*iter).Full_name);

			if(strcmp(str[i],buf) == 0)
			{
				persons.push_back((*iter).Fix_register_id);
				m_vecexamroster.erase(iter);
				break;
			}
			iter ++;
		}
	}
	delete[] str;

	if (!flag)
	{
		reverse(persons.begin(), persons.end());
	}

}


//枪手库
bool InstanceDB::AddImposer(string version, string guid, int size, char* data)
{
	char cmd[1024];
	sprintf(cmd,"INSERT INTO T_GHOST_LIBRARY VALUES ('%s','%s',%d,'%s')",
				version.data(),guid.data(),size,data);

	if(execute(cmd))
		return true;
	return false;
}


bool InstanceDB::GetImposer(string version, vector<string/*guid*/>& imposer)
{

	char cmd[1024];
	sprintf(cmd, "SELECT * FROM T_GHOST_LIBRARY WHERE ghost_library_version = '%s'", version.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		string str;
		str = set.GetAsString(L"ghost_id").mb_str();
		imposer.push_back(str);
	}

// 	if(imposer.size() == 0)
// 		return false;

	return true;
}


bool InstanceDB::GetImposerInfo(string version, string guid,GHOST_LIBRARY& ghost_library/*所有字段组成的结构体*/)
{
	char cmd[1024];
	sprintf(cmd, "SELECT * FROM T_GHOST_LIBRARY WHERE ghost_library_version = '%s' AND ghost_id = '%s'", version.data(),guid.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{

		ghost_library.Ghost_feature_size = set.GetInt(L"ghost_feature_size");
		strcpy(ghost_library.Ghost_feature_data,set.GetAsString(L"ghost_feature_data").mb_str());
	}
	return true;
}

bool InstanceDB::DelImposer(string version, string guid)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_GHOST_LIBRARY WHERE ghost_library_version='%s'AND ghost_id='%s'",version.data(),guid.data());
	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::DelExamImposer(string version)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_GHOST_LIBRARY WHERE ghost_library_version='%s'",version.data());
	if(execute(cmd))
		return true;
	return false;

}


//全局配置表
bool InstanceDB::GetGlobalInfo(GLOBAL_CONFIG& global_config)
{
	char cmd[1024];
	sprintf(cmd, "SELECT * FROM T_GLOBAL_CONFIG");
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		strcpy(global_config.Application_version,set.GetAsString(L"application_version").mb_str());
		strcpy(global_config.Front_ip,set.GetAsString(L"front_ip").mb_str());
		global_config.Front_port = set.GetInt(L"front_port"); 
		strcpy(global_config.Front_domain_name,set.GetAsString(L"front_domain_name").mb_str());
		strcpy(global_config.Sftp_ip,set.GetAsString(L"sftp_ip").mb_str());
		global_config.Sftp_port = set.GetInt(L"sftp_port"); 
		strcpy(global_config.Sftp_domain_name,set.GetAsString(L"sftp_domain_name").mb_str());
		strcpy(global_config.Login_time,set.GetAsString(L"login_time").mb_str());
		global_config.Threshold_person_licence = set.GetInt(L"threshold_person_licence");
		global_config.Threshold_person_person = set.GetInt(L"threshold_person_person");
		global_config.Threshold_history = set.GetInt(L"threshold_history");
		global_config.Threshold_ghost = set.GetInt(L"threshold_ghost");
		strcpy(global_config.Exam_site,set.GetAsString(L"exam_site").mb_str());
		strcpy(global_config.Exam_room,set.GetAsString(L"exam_room").mb_str());
	}
	return true;
}

bool InstanceDB::SetGlobalInfo(GLOBAL_CONFIG global_config)
{

	char cmd[1024];
	sprintf(cmd,"UPDATE T_EXAM_ROOM set application_version='%s', front_ip='%s',front_port = %d,front_domain_name = '%s' \
				sftp_ip = '%s',sftp_port=%d,sftp_domain_name='%s',login_time='%s',threshold_person_licence=%d,\
				threshold_person_person = %d,threshold_history=%d,threshold_ghost=%d,exam_site='%s',exam_room='%s'",
			global_config.Application_version,global_config.Front_ip,global_config.Front_port,global_config.Front_domain_name,
			global_config.Sftp_ip,global_config.Sftp_port,global_config.Sftp_domain_name,global_config.Login_time,
			global_config.Threshold_person_licence,global_config.Threshold_person_person,global_config.Threshold_history,
			global_config.Threshold_ghost,global_config.Exam_site,global_config.Exam_room);
	if(execute(cmd))
		return true;
	return false;
}

//证件表
bool InstanceDB::AddLicence(string version, int value, string desc)
{
	char cmd[1024];
	sprintf(cmd,"INSERT INTO T_LICENCE_TYPE VALUES ('%s',%d,'%s')",
			version.data(),value,desc.data());

	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::GetLicence(string version, vector<LICENCE_TYPE>& vLicence_type)
{
	char cmd[1024];
	LICENCE_TYPE licence_type;
	sprintf(cmd, "SELECT * FROM T_LICENCE_TYPE WHERE licence_type_version='%s'",version.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		
		strcpy(licence_type.Discription,set.GetAsString(L"discription").mb_str());

		licence_type.Value = set.GetInt(L"value");
		vLicence_type.push_back(licence_type);
	}

// 	if(vLicence_type.size() == 0)
// 		return false;

	return true;
}

bool InstanceDB::DelExamLicence(string version)
{

	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_LICENCE_TYPE WHERE licence_type_version='%s'",version.data());
	if(execute(cmd))
		return true;
	return false;
}


//匹配配置表
bool InstanceDB::AddMatchConfig(string exam_id, MATCH_CONFIG match_config)
{
	char cmd[1024];
	sprintf(cmd,"INSERT INTO T_MATCH_CONFIG VALUES ('%s',%d,%d,%d,%d,'%s',%d)",
			exam_id.data(),match_config.Threshold_person_licence,match_config.Threshold_person_person,
			match_config.Threshold_history,match_config.Threshold_ghost,match_config.Secret_key,match_config.Mode);

	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::UpdateMatchConfig(string exam_id, MATCH_CONFIG match_config)
{
	char cmd[1024];
	sprintf(cmd,"UPDATE T_MATCH_CONFIG set threshold_person_licence=%d, threshold_person_person=%d,threshold_history =%d,threshold_ghost =%d,secret_key='%s',mode = %d WHERE exam_id='%s'",
			match_config.Threshold_person_licence,match_config.Threshold_person_person,match_config.Threshold_history,
			match_config.Threshold_ghost,match_config.Secret_key,match_config.Mode,exam_id.data());
	if(execute(cmd))
		return true;
	return false;

}

bool InstanceDB::DelMatchConfig(string exam_id)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_MATCH_CONFIG WHERE exam_id='%s'",exam_id.data());
	if(execute(cmd))
		return true;
	return false;
}


bool InstanceDB::QueryMatchConfig(string exam_id, MATCH_CONFIG& match_config)
{

	char cmd[1024];
	sprintf(cmd, "SELECT * FROM T_MATCH_CONFIG WHERE exam_id = '%s'", exam_id.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		match_config.Threshold_person_licence = set.GetInt(L"threshold_person_licence");
		match_config.Threshold_person_person = set.GetInt(L"threshold_person_person");
		match_config.Threshold_history = set.GetInt(L"threshold_history");
		match_config.Threshold_ghost = set.GetInt(L"threshold_ghost");
		strcpy(match_config.Secret_key,set.GetAsString(L"secret_key").mb_str());
		match_config.Mode = set.GetInt(L"mode");
	}
	return true;
}

//版本控制表
bool InstanceDB::AddVersionControl(string exam_id, VERSION_CONTROL version_control)
{
	char cmd[1024];
	sprintf(cmd,"INSERT INTO T_VERSION_CONTROL VALUES ('%s','%s','%s','%s','%s')",
		version_control.Exam_id,version_control.Licence_type_version,version_control.Event_type_version,
		version_control.Ghost_library_version,version_control.Exam_roster_version);

	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::UpdateVersionControl(string exam_id, VERSION_CONTROL version_control)
{
	char cmd[1024];
	sprintf(cmd,"UPDATE T_VERSION_CONTROL set licence_type_version='%s', event_type_version='%s',ghost_library_version ='%s',exam_roster_version = '%s'WHERE exam_id='%s'",
		version_control.Licence_type_version,version_control.Event_type_version,version_control.Ghost_library_version,version_control.Exam_roster_version,exam_id.data());
	if(execute(cmd))
		return true;
	return false;
}

bool InstanceDB::QueryVersionControl(string exam_id, VERSION_CONTROL& version_control)
{
	char cmd[1024];
	sprintf(cmd, "SELECT * FROM T_VERSION_CONTROL WHERE exam_id = '%s'", exam_id.data());
	wxSQLite3ResultSet set = db->ExecuteQuery(cmd);
	while(set.NextRow())
	{
		strcpy(version_control.Licence_type_version,set.GetAsString(L"licence_type_version").mb_str());
		strcpy(version_control.Event_type_version,set.GetAsString(L"event_type_version").mb_str());
		strcpy(version_control.Ghost_library_version,set.GetAsString(L"ghost_library_version").mb_str());
		strcpy(version_control.Exam_roster_version,set.GetAsString(L"exam_roster_version").mb_str());
	}
	return true;
}

bool InstanceDB::DelVersionControl(string exam_id)
{
	char cmd[1024];
	sprintf(cmd,"DELETE FROM T_VERSION_CONTROL WHERE exam_id='%s'",exam_id.data());

	BeginTransaction();

	if(execute(cmd))
	{
		if(!DelExamRoster(exam_id,false))
			return false;
 		if(!DelExamEntrance(exam_id))
 			return false;
		if(!DelExamReport(exam_id,false))
			return false;
 		if(!DelExamAttachment(exam_id))
 			return false;
		CommitTransaction();
		return true;
	}
	else
		RollbackTransaction();

	return false;
}