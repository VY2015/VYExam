#ifndef  ROSTER_H
#define  ROSTER_H

#ifndef INFOMATION_H
#include "Infomation.h"
#endif

#include<map>
#include <list>
#include <vector>
#include "wx//wxsqlite3.h"
#include "wx/wxprec.h"
#include <ctime>
#include <iostream>
#include <process.h>
#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

using namespace std;
class wxSQLite3Database;
class _declspec(dllexport)  Roster
{
public:
	Roster();
	~Roster();
	bool Init(wxString strRosterPath,wxString key); //��ʼ��·������������
	void OpendEncrypted(wxString strRosterPath,wxString key);//�򿪼������ݿ�
	void ChangeKey(wxString key);//��������
	//bool Init(wxString strRosterPath);
// bool read(string strRosterPath);//
	void save(string strRosterPath);//����·��
	bool insert(FreshInfo );
	bool deleteInfo(int );//ID����Int
	bool deleteInfo(char *ID);//ID����char*
	bool queryInfo(int, FreshInfo &);
	FreshInfo queryInfo(int );//������int��ID
	FreshInfo queryInfo(char *); //char*������ID��������
	bool changeInfo(int, FreshInfo *);//������Ϣ��ID��int
	bool changeInfo(char *, FreshInfo &);//������Ϣ��ID��char*
	void display(FreshInfo  fre);
	wstring s2ws(const string& s);//stringת��wxstring
	std::string Unicode2Utf8(const std::wstring& widestring);//Unicodeת��Utf8
private:
	wxSQLite3Database *db;
//	map<string, int>  m_mapPerson;
	FreshInfo * m_FreshInfo;
	bool IsNumString(string str); //�ж��������ַ����Ƿ������ַ�
	
};

#endif
