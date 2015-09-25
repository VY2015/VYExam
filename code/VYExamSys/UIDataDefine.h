#pragma once
#include "..\DuiLib\UIlib.h"
#include <Windows.h>
#include "DataDefine.h"


#define WM_ADDLISTITEM WM_USER + 50
#define ID_NEWMENU     WM_USER + 101


struct Prama
{
	HWND hWnd;
	CListUI* pList;
};
