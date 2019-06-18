#pragma once
#include <windows.h>

/**
 * 从 google omaha 中参考
 *   omaha/base/service_util.h/cc
 *
 *
 */
class ScmDatabase {
public:
    typedef HRESULT(*EnumerateServicesCallback)(void* callback_context,
                                                const wchar_t* service_name);

    static HRESULT EnumerateServices(EnumerateServicesCallback callback,
                                     void* callback_context);

    static bool IsServiceStateEqual(SC_HANDLE service, DWORD state);

    static bool IsServiceMarkedDeleted(SC_HANDLE service);
};

class ServiceInstall {
public:
    static HRESULT InstallServices(
        const TCHAR* service_name,
        const TCHAR* service_display_name,
        const TCHAR* service_cmd_line,
        int service_type,
        int service_start_type);


    static HRESULT UpgradeService(const TCHAR* service_name, const TCHAR* service_cmd_line, int service_start_type);
    static bool IsServiceCorrectlyConfigured(const TCHAR* service_name,
                                             const TCHAR* service_cmd_line, int service_start_type);

    static HRESULT UninstallServices(const TCHAR* service_name);

    static bool IsServiceInstalled(const TCHAR* service_name);
    static bool CanInstallWithoutReboot();

    static HRESULT StartService(const TCHAR* service_name);
    static HRESULT StopService(const TCHAR* service_name);

    static HRESULT SetDescription(const TCHAR* service_name, const TCHAR* description);
    static bool IsDelayedAutoStart(const TCHAR* service_name);

    static HRESULT SetDelayedAutoStart(const TCHAR* service_name);

private:
    static HRESULT DoInstallNewService(
        const TCHAR* service_name,
        const TCHAR* service_display_name,
        const TCHAR* service_cmd_line,
        int service_type,
        int service_start_type);
};

class ServiceUtils {
public:
    static bool IsServiceRunning(const TCHAR* service_name);
    static bool IsServiceDisabled(const TCHAR* service_name);
};