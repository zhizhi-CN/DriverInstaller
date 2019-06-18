// DriverInstaller.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DriverInstaller.h"
#include "dlg_main.h"

#include <base/at_exit.h>
#include <base/command_line.h>
#include <base/message_loop/message_loop.h>
#include <base/win/windows_version.h>

#include <commdlg.h>


#pragma comment(lib, "Comctl32.lib")


BOOL try_ChangeWindowMessageFilter(UINT message, DWORD dwFlag) {
    auto fn = reinterpret_cast<decltype(::ChangeWindowMessageFilter)*>(
        GetProcAddress(GetModuleHandle(L"user32"), 
                       "ChangeWindowMessageFilter"));
    if (!fn) {
        return FALSE;
    }

    return fn(message, dwFlag);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    OleInitialize(nullptr);

    if (base::win::GetVersion() >= base::win::VERSION_VISTA) {        
        try_ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
        try_ChangeWindowMessageFilter(WM_COPYDATA, MSGFLT_ADD);
        try_ChangeWindowMessageFilter(0x0049/* WM_COPYGLOBALDATA*/, MSGFLT_ADD);
    }

    base::AtExitManager at_exit;
    base::CommandLine::Init(0, nullptr);
    base::MessageLoopForUI loop;

    DlgMain dlg_main;
    dlg_main.Create(nullptr);
    dlg_main.CenterWindow();
    dlg_main.ShowWindow(SW_NORMAL);

    loop.Run();
    OleUninitialize();
    return 0;
}