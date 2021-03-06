#pragma once
#include "header.h"

class CMultithreadDlg
{
public:
	CMultithreadDlg(void);
public:
	~CMultithreadDlg(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CMultithreadDlg* ptr;
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void CMultithreadDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	HWND hEdit1, hEdit2, hButton1, hButton2, hProgress, hCopy;
};
