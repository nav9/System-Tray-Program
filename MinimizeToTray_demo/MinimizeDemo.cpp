// MinimizeToTray
//
// A sample app to show how to make it produce a custom caption animation to
// make it look like we are minimizing to and maximizing from the system tray
//
// Copyright 2000 Matthew Ellis <m.t.ellis@bigfoot.com>
#include "stdafx.h"
#include "resource.h"

#include "MinimizeToTray.h"

#include <iostream>
#include<sstream>
#include <ctime>
#include<process.h>//for _beginthread
using namespace std;
time_t startTime;
bool firstTime = true;
const double REST_INTERVAL_IN_SECOND = 45 * 60;//minutes * seconds in a minute
const double MILLISECOND = 1000;

HINSTANCE g_hInstance;

#define WM_TRAYMESSAGE WM_USER

static VOID ShowNotifyIcon(HWND hWnd,BOOL bAdd)
{
  NOTIFYICONDATA nid;
  ZeroMemory(&nid,sizeof(nid));
  nid.cbSize=sizeof(NOTIFYICONDATA);
  nid.hWnd=hWnd;
  nid.uID=0;
  nid.uFlags=NIF_ICON | NIF_MESSAGE | NIF_TIP;
  nid.uCallbackMessage=WM_TRAYMESSAGE;
  nid.hIcon=LoadIcon(g_hInstance,MAKEINTRESOURCE(IDI_ICON1));
  lstrcpy(nid.szTip,TEXT("Double-click to maxmize!"));

  if(bAdd)
    Shell_NotifyIcon(NIM_ADD,&nid);
  else
    Shell_NotifyIcon(NIM_DELETE,&nid);
}

static BOOL CALLBACK DialogProc(HWND hWnd,UINT uiMsg,WPARAM wParam,LPARAM lParam)
{
  static bool bHideIcon=false;

  switch(uiMsg)
  {
    case WM_SYSCOMMAND:
      if(wParam==SC_MINIMIZE)
      {
	MinimizeWndToTray(hWnd);
	ShowNotifyIcon(hWnd,TRUE);

	// Return TRUE to tell DefDlgProc that we handled the message, and set
	// DWL_MSGRESULT to 0 to say that we handled WM_SYSCOMMAND
	SetWindowLong(hWnd,DWL_MSGRESULT,0);
	return TRUE;
      }
      break;

    case WM_TRAYMESSAGE:
      switch(lParam)
      {
	case WM_LBUTTONDBLCLK:
	  RestoreWndFromTray(hWnd);
	  // If we remove the icon here, the following WM_LBUTTONUP (the user
	  // releasing the mouse button after a double click) can be sent to
	  // the icon that occupies the position we were just using, which can
	  // then activate, when the user doesn't want it to. So, set a flag
	  // so that we remove the icon on the next WM_LBUTTONUP
	  bHideIcon=true;
	  return TRUE;

	case WM_LBUTTONUP:
	  // The user has previously double-clicked the icon, remove it in
	  // response to this second WM_LBUTTONUP
	  if(bHideIcon)
	  {
	    ShowNotifyIcon(hWnd,FALSE);
	    bHideIcon=false;
	  }
	  return TRUE;
      }
      break;

    case WM_CLOSE:
      MinimizeWndToTray(hWnd);
      ShowNotifyIcon(hWnd,TRUE);
      return TRUE;

    case WM_COMMAND:
      PostQuitMessage(0);
      break;
  }

  return FALSE;
}


void checkEyeRestTime(void* ptr)
{
    while( true )
    {
        if (firstTime) {firstTime=false; Sleep(REST_INTERVAL_IN_SECOND * MILLISECOND);}
        time_t now = time(0);// current date/time based on current system
        char* dt = ctime(&now);// convert now to string form
        //cout << "The local date and time is: " << dt << endl;
        //tm *gmtm = gmtime(&now);dt = asctime(gmtm);// convert now to tm struct for UTC
        //cout << "The UTC date and time is:"<< dt << endl;
        double dif = difftime( time(0), startTime );
        //std::ostringstream strs;
        //strs << dif;
        //std::string str = strs.str();
        //LPSTR s = const_cast<char *>(str.c_str());

        if (dif > REST_INTERVAL_IN_SECOND) 
        {
            MessageBox(NULL, "Time to rest your eyes Navin", "Hey!", MB_ICONINFORMATION|MB_SETFOREGROUND);
            startTime = time(0);
            Sleep(REST_INTERVAL_IN_SECOND * MILLISECOND);//second * millisecond conversion for the second
        }//if
    }//while

    _endthread();
}//checkresttime

// Simple entry point and message pump
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow)
{
  startTime = time(0);
  g_hInstance=hInstance;

  HWND hWnd=CreateDialog(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,DialogProc);
  if(hWnd)
  {
    MSG msg;
    _beginthread(&checkEyeRestTime, 0, 0);  
    while(GetMessage(&msg,hWnd,0,0))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return 0;
}

