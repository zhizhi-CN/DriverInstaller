#include "stdafx.h"
#include "dlg_main.h"



#include <commdlg.h>

#include <base/strings/stringprintf.h>

LRESULT DlgMain::OnInitialDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) { 
    HICON hIconBig = (HICON)::LoadImage(GetModuleHandle(NULL), 
                                        MAKEINTRESOURCE(IDI_DRIVERINSTALLER),
                                        IMAGE_ICON, 
                                        ::GetSystemMetrics(SM_CXICON), 
                                        ::GetSystemMetrics(SM_CYICON), 
                                        LR_DEFAULTCOLOR);

    HICON hIconSmall = (HICON)::LoadImage(GetModuleHandle(NULL),
                                          MAKEINTRESOURCE(IDI_DRIVERINSTALLER),
                                          IMAGE_ICON, 
                                          ::GetSystemMetrics(SM_CXSMICON), 
                                          ::GetSystemMetrics(SM_CYSMICON), 
                                          LR_DEFAULTCOLOR);

    SendMessage(m_hWnd, WM_SETICON, TRUE, (LPARAM)hIconBig);
    SendMessage(m_hWnd, WM_SETICON, FALSE, (LPARAM)hIconSmall);

    ::DragAcceptFiles(m_hWnd, TRUE);
    UpdateUI();
    return 0L;
}

LRESULT DlgMain::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    CloseWindow(m_hWnd);
    DestroyWindow();
    PostQuitMessage(0x00);
    return 0;
}

LRESULT DlgMain::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {

    bool success = false;
    WCHAR filename[MAX_PATH];
    success = !!DragQueryFileW((HDROP)wParam, 0, filename, MAX_PATH);
    DragFinish((HDROP)wParam);
    if (!PathIsDirectoryW(filename)) {
        OpenFile(base::FilePath(filename));
    }
    return 0L;
}

LRESULT DlgMain::OnBnClickedAwaysTopmost(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
    if (IsDlgButtonChecked(IDC_AWAYS_TOPMOST)) {
        SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }
    else {
        SetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }
    return 0;
}


LRESULT DlgMain::OnBnClickedOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
    wchar_t drv_path[MAX_PATH];
    wcscpy_s(drv_path, MAX_PATH, L"");

    OPENFILENAMEW file_info = { 0 };
    file_info.lStructSize = sizeof(file_info);
    file_info.hwndOwner = m_hWnd;
    file_info.lpstrFile = drv_path;
    file_info.nMaxFile = MAX_PATH;
    file_info.lpstrFilter = L"DriverFiles\0*.sys\0";
    file_info.nFilterIndex = 0;
    file_info.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameW(&file_info)) {
        GetDlgItem(IDC_EDIT_PATH).SetWindowText(drv_path);
        driver_service_.SetDrvPath(base::FilePath(drv_path));
        UpdateUI();
    }
    return 0;
}


LRESULT DlgMain::OnBnClickedClost(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
    CloseWindow(m_hWnd);
    DestroyWindow();
    PostQuitMessage(0x00);
    return 0;
}

LRESULT DlgMain::OnBnClickedInstall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {    
    if (driver_service_.Install()) {
        SetStatusText(L"驱动安装成功");
    }
    else {
        SetStatusText(base::StringPrintf(L"驱动安装失败[0x%08x]", driver_service_.LastHRESULT()));
    }
    UpdateUI();
    return 0;
}



LRESULT DlgMain::OnBnClickedStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
    if (driver_service_.Start()) {
        SetStatusText(L"驱动启动成功");
    }
    else {
        SetStatusText(base::StringPrintf(L"驱动启动失败[0x%08x]", driver_service_.LastHRESULT()));
    }
    UpdateUI();
    return 0;
}


LRESULT DlgMain::OnBnClickedStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
    if (driver_service_.Stop()) {
        SetStatusText(L"驱动停止成功");
    }
    else {
        SetStatusText(base::StringPrintf(L"驱动停止失败[0x%08x]", driver_service_.LastHRESULT()));
    }
    UpdateUI();
    return 0;
}

LRESULT DlgMain::OnBnClickedUninst(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
    if (driver_service_.Uninsall()) {
        SetStatusText(L"驱动卸载成功");
    }
    else {
        SetStatusText(base::StringPrintf(L"驱动卸载失败[0x%08x]", driver_service_.LastHRESULT()));
    }
    UpdateUI();
    return 0;
}

void DlgMain::SetStatusText(const std::wstring& status_text) {
    GetDlgItem(IDC_STATUS).SetWindowText(base::StringPrintf(L"状态：%ls", status_text.c_str()).c_str());
}

void DlgMain::UpdateUI() {
    switch (driver_service_.state()) {
    case DriverService::DRV_INVALID:
        GetDlgItem(IDC_INSTALL).EnableWindow(FALSE);
        GetDlgItem(IDC_UNINST).EnableWindow(FALSE);
        GetDlgItem(IDC_START).EnableWindow(FALSE);
        GetDlgItem(IDC_STOP).EnableWindow(FALSE);
        break;
    case DriverService::DRV_NONE: 
        GetDlgItem(IDC_INSTALL).EnableWindow(TRUE);
        GetDlgItem(IDC_UNINST).EnableWindow(FALSE);
        GetDlgItem(IDC_START).EnableWindow(FALSE);
        GetDlgItem(IDC_STOP).EnableWindow(FALSE);
        break;
    case DriverService::DRV_RUNING:
        GetDlgItem(IDC_INSTALL).EnableWindow(FALSE);
        GetDlgItem(IDC_UNINST).EnableWindow(TRUE);
        GetDlgItem(IDC_START).EnableWindow(FALSE);
        GetDlgItem(IDC_STOP).EnableWindow(TRUE);
        break;
    case DriverService::DRV_STOPED:
        GetDlgItem(IDC_INSTALL).EnableWindow(FALSE);
        GetDlgItem(IDC_UNINST).EnableWindow(TRUE);
        GetDlgItem(IDC_START).EnableWindow(TRUE);
        GetDlgItem(IDC_STOP).EnableWindow(FALSE);
        break;
    default:
        break;
    }
}

void DlgMain::OpenFile(const base::FilePath& file) {

}
