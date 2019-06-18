#pragma once

#include <base/files/file_path.h>

class DriverService {
public:
    enum STATE {
        DRV_INVALID,
        DRV_NONE,
        DRV_RUNING,
        DRV_STOPED,
    };

    DriverService();

    void SetDrvPath(const base::FilePath& path);

    std::wstring DrvName();

    bool Install();
    bool Uninsall();

    bool Start();
    bool Stop();

    STATE state() const { return state_; }

    HRESULT LastHRESULT() { return last_hresult_; }

private:
    STATE state_;
    base::FilePath drv_path_;

    HRESULT last_hresult_;
};
