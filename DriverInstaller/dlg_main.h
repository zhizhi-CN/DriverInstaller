#pragma once

#include <shellapi.h>

#include <base/files/file_path.h>

#include "DriverService.h"

class DlgMain;

class DlgMain : public CDialogImpl<DlgMain> {
public:
    enum {IDD = IDD_DLG_MAIN };

protected:
    BEGIN_MSG_MAP(DlgMain)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitialDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)
        COMMAND_HANDLER(IDC_AWAYS_TOPMOST, BN_CLICKED, OnBnClickedAwaysTopmost)
        COMMAND_HANDLER(IDC_OPEN, BN_CLICKED, OnBnClickedOpen)
        COMMAND_HANDLER(IDC_CLOST, BN_CLICKED, OnBnClickedClost)
        COMMAND_HANDLER(IDC_INSTALL, BN_CLICKED, OnBnClickedInstall)
        COMMAND_HANDLER(IDC_START, BN_CLICKED, OnBnClickedStart)
        COMMAND_HANDLER(IDC_STOP, BN_CLICKED, OnBnClickedStop)
        COMMAND_HANDLER(IDC_UNINST, BN_CLICKED, OnBnClickedUninst)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

public:
    LRESULT OnInitialDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    
    LRESULT OnBnClickedAwaysTopmost(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedClost(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedInstall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    
    void SetStatusText(const std::wstring& status_text);

    void UpdateUI();

    void OpenFile(const base::FilePath& file);

private:
    DriverService driver_service_;
public:
    LRESULT OnBnClickedStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedUninst(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
