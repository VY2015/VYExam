#ifndef __VY_EXAM_FLIE_DEF_H__
#define __VY_EXAM_FLIE_DEF_H__
#include <string>


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#define  VLENGTH 50;

typedef struct _Report_List_Info_
{
	char	Name[1024];
	char	Attend[1024];         //出席状态
	char	RegisterId[1024];
	char	Seat[1024];
	char	Times[1024];

}Report_List_Info;

typedef struct _EventNode_
{
	int		Own_value;
	char	Dsec[50];
} EventNode;


typedef struct _EXAM_ENTRANCE_
{
	char	Exam_id[50];
	char	Fix_register_id[50];
	char	Entrance_time[50];
	char	Entrance_photo_path[150];
	int		Ghost_check;
	int		Machine_check;
	int		Man_check;
	int		History_check;
	int		Times;
}EXAM_ENTRANCE;

typedef struct _EXAM_REPORT_
{
	char   Exam_id[50];
	int	   Report_type;
	char   Occurrence_time[50];
	std::string People;
	std::string Detail;
	char   Attachment_guid[50];

}EXAM_REPORT;


typedef struct _EXAM_ROOM_
{
	char   Exam_id[50];
	char   Chief_examiner_id[50];
	char   Chief_examiner_name[50];
	char   Assistant_examiner_id[50];
	char   Assistant_examiner_name[50];

}EXAM_ROOM;


typedef struct _EXAM_ROSTER_
{
	char	Exam_id[50];
	char	Fix_register_id[50];
	int		Fix_licence_type;
	char	Fix_chs_surname[50];
	char	Fix_chs_name[50];
	char	Fix_en_surname[50];
	char	Fix_en_name[50];
	char	Fix_nation[50];
	char	Fix_sex[2];
	char	Fix_born[50];
	char	Fix_addr[50];
	char	Fix_card_num[50];
	char	Fix_police[50];
	char	Fix_active[50];
	char	Full_name[50];
	char	Nation[50];
	char	Sex[2];
	char	Born[50];
	char	Addr[50];
	char	Card_num[50];
	char	Police[50];
	char	Active[50];
	int		Attend;         //是否缺席
	char	Seat[50];
	char	Licence_photo_path[150];
	char	Head_photo_path[150];
	char	Fix_history_photo_path[150];
	int		Times;

}EXAM_ROSTER;


typedef struct _GHOST_LIBRARY_
{
	char	Ghost_library_version[10];
	char	Ghost_id[32];
	int		Ghost_feature_size;
	char	Ghost_feature_data[3096];
}GHOST_LIBRARY;


typedef struct _GLOBAL_CONFIG_
{
	char	Application_version[10];
	char	Front_ip[15];
	int		Front_port;
	char	Front_domain_name[100];
	char	Sftp_ip[15];
	int		Sftp_port;
	char	Sftp_domain_name[100];
	char	Login_time[50];
	int		Threshold_person_licence;
	int		Threshold_person_person;
	int		Threshold_history;
	int		Threshold_ghost;
	char	Exam_site[50];
	char	Exam_room[50];

	//字段是否完整
}GLOBAL_CONFIG;


typedef struct _LICENCE_TYPE_
{
	char	Licence_type_version[10];
	int		Value;
	char	Discription[50];
}LICENCE_TYPE;

typedef struct _MATCH_CONFIG_
{
	char	Exam_id[50];
	int		Threshold_person_licence;
	int		Threshold_person_person;
	int		Threshold_history;
	int		Threshold_ghost;
	char	Secret_key[2048];
	int		Mode;
}MATCH_CONFIG;


typedef struct _VERSION_CONTROL_
{
	char	Exam_id[50];
	char	Exam_roster_version[10];
	char	Licence_type_version[10];
	char	Event_type_version[10];
	char	Ghost_library_version[10];

}VERSION_CONTROL;


typedef struct _VYPersonInfo_
{
	char			_stRegister_id;
	EXAM_ENTRANCE	_stEntrance;
	EXAM_ROSTER		_stRoster;

}VYPersonInfo;


typedef enum _ROSTER_SORTKEY_{
	VY_SORT_NAME,
	VY_SORT_ATTEND,    	
	VY_SORT_MANCHECK,
	VY_SORT_REGISTER_ID,
	VY_SORT_SEATS,
	VY_SORT_IMPOSTERS,
	VY_SORT_HISPHOTOS,
	VY_SORT_TIMES
}ROSTER_SORTKEY;


#endif