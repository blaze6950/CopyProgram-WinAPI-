#include "MultithreadDlg.h"
#include <fstream>
#include <iostream>
#include "shlobj.h"

CMultithreadDlg* CMultithreadDlg::ptr = NULL;

void saveFile(HWND hwnd);
void openFile(HWND hwnd);
void copyFile(HWND hwnd);

CMultithreadDlg::CMultithreadDlg(void)
{
	ptr = this;
}

CMultithreadDlg::~CMultithreadDlg(void)
{

}

void CMultithreadDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

DWORD WINAPI Thread1(LPVOID lp)
{
	HWND hwnd = (HWND)lp;
	TCHAR str1[_MAX_PATH];
	TCHAR str2[_MAX_PATH];
	SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_GETTEXT, LPARAM(str1), 0);
	SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_GETTEXT, LPARAM(str2), 0);

	CopyFile(LPCWSTR(str1), LPCWSTR(str2), TRUE);
	return 0;
}
DWORD WINAPI Thread2(LPVOID lp)
{
	HWND hList = (HWND)lp;
	for(int i = 0; i < 20; i++)
	{
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Работает второй поток")));
		Sleep(10);
	}
	SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Второй поток завершает работу")));
	return 0;
}
DWORD WINAPI Thread3(LPVOID lp)
{
	HWND hList = (HWND)lp;
	for(int i = 0; i < 10; i++)
	{
		SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Работает третий поток")));
		Sleep(10);
	}
	SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Третий поток завершает работу")));
	return 0;
}

BOOL CMultithreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
	hEdit2 = GetDlgItem(hwnd, IDC_EDIT2);
	hButton1 = GetDlgItem(hwnd, IDC_BUTTON1);
	hButton2 = GetDlgItem(hwnd, IDC_BUTTON2);
	hProgress = GetDlgItem(hwnd, IDC_PROGRESS1);
	hCopy = GetDlgItem(hwnd, IDC_BUTTON3);
	/*CreateThread(NULL, 0, Thread1, hList, 0, NULL);
	CreateThread(NULL, 0, Thread2, hList, 0, NULL);
	CreateThread(NULL, 0, Thread3, hList, 0, NULL);*/

	return TRUE;
}

void CMultithreadDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_BUTTON1:
		openFile(hwnd);
		break;
	case IDC_BUTTON2:
		saveFile(hwnd);
		break;
	case IDC_BUTTON3:
		copyFile(hwnd);
		break;
	}
}

BOOL CALLBACK CMultithreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}

void saveFile(HWND hwnd)
{
	//TCHAR FullPath[MAX_PATH] = { 0 };
	//OPENFILENAME open = { sizeof(OPENFILENAME) };
	//open.hwndOwner = hwnd;
	//open.lpstrFilter = TEXT("All Files(*.*)\0*.*\0");
	//open.lpstrFile = FullPath;
	//open.nMaxFile = MAX_PATH;
	//open.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
	////open.lpstrDefExt = TEXT("txt");
	//if (GetSaveFileName(&open))
	//{
	//	SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, LPARAM(FullPath));
	//}

	TCHAR buffer[_MAX_PATH] = { '\0' };
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.ulFlags = BIF_USENEWUI;
	bi.hwndOwner = hwnd;
	bi.lpszTitle = NULL;

	// must call this if using BIF_USENEWUI
	::OleInitialize(NULL);

	// Show the dialog and get the itemIDList for the 
	// selected folder.
	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);

	if (pIDL != NULL)
	{
		// Create a buffer to store the path, then 
		// get the path.
		if (::SHGetPathFromIDList(pIDL, buffer) != 0)
		{
			// Set the string value.
			SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, LPARAM(buffer));
		}

		// free the item id list
		CoTaskMemFree(pIDL);
	}

	::OleUninitialize();
}

void openFile(HWND hwnd)
{
	TCHAR FullPath[MAX_PATH] = { 0 };
	OPENFILENAME open = { sizeof(OPENFILENAME) };
	open.hwndOwner = hwnd;
	open.lpstrFilter = TEXT("All Files(*.*)\0*.*\0");
	open.lpstrFile = FullPath;
	open.nMaxFile = MAX_PATH;
	open.lpstrInitialDir = TEXT("C:\\");
	open.Flags = OFN_CREATEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	if (GetOpenFileName(&open))
	{
		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, LPARAM(FullPath));
	}
	
}

void copyFile(HWND hwnd)
{
	CreateThread(NULL, 0, Thread1, hwnd, 0, NULL);
}