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
	bool Init(wxString strRosterPath,wxString key); //初始化路径，设置密码
	void OpendEncrypted(wxString strRosterPath,wxString key);//打开加密数据库
	void ChangeKey(wxString key);//更改密码
	//bool Init(wxString strRosterPath);
// bool read(string strRosterPath);//
	void save(string strRosterPath);//保存路径
	bool insert(FreshInfo );
	bool deleteInfo(int );//ID号是Int
	bool deleteInfo(char *ID);//ID号是char*
	bool queryInfo(int, FreshInfo &);
	FreshInfo queryInfo(int );//输入是int型ID
	FreshInfo queryInfo(char *); //char*可以是ID或者姓名
	bool changeInfo(int, FreshInfo *);//更改信息，ID是int
	bool changeInfo(char *, FreshInfo &);//更改信息，ID是char*
	void display(FreshInfo  fre);
	wstring s2ws(const string& s);//string转成wxstring
	std::string Unicode2Utf8(const std::wstring& widestring);//Unicode转成Utf8
private:
	wxSQLite3Database *db;
//	map<string, int>  m_mapPerson;
	FreshInfo * m_FreshInfo;
	bool IsNumString(string str); //判断是数字字符还是非数字字符
	
};

#endif
