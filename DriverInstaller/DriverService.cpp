#include "stdafx.h"
#include "DriverService.h"

#include "service_utils.h"

DriverService::DriverService() : state_(DRV_INVALID), last_hresult_(ERROR_SUCCESS) {

}

void DriverService::SetDrvPath(const base::FilePath& path) {
    drv_path_ = path;
    state_ = DRV_NONE;

    if (ServiceInstall::IsServiceInstalled(DrvName().c_str())) {
        if (ServiceUtils::IsServiceRunning(DrvName().c_str())) {
            state_ = DRV_RUNING;
        }
        else {
            state_ = DRV_STOPED;
        }
    }
}

std::wstring DriverService::DrvName() {
    return L"DI_" + drv_path_.BaseName().RemoveExtension().value();
}

bool DriverService::Install() {
    if (ServiceInstall::IsServiceInstalled(DrvName().c_str())) {
        return true;
    }

    last_hresult_ = ServiceInstall::InstallServices(
        DrvName().c_str(),
        DrvName().c_str(),
        drv_path_.value().c_str(),
        SERVICE_KERNEL_DRIVER,
        SERVICE_DEMAND_START);

    if (FAILED(last_hresult_)) {
        return false;
    }

    state_ = DRV_STOPED;
    return true;
}

bool DriverService::Uninsall() {
    last_hresult_ = ServiceInstall::UninstallServices(DrvName().c_str());
    if (FAILED(last_hresult_)) {
        return false;
    }
    state_ = DRV_NONE;
    return true;
}

bool DriverService::Start() {
    last_hresult_ = ServiceInstall::StartService(DrvName().c_str());
    if (SUCCEEDED(last_hresult_)) {
        state_ = DRV_RUNING;
    }
    return SUCCEEDED(last_hresult_);
}

bool DriverService::Stop() {
    last_hresult_ = ServiceInstall::StopService(DrvName().c_str());
    if (SUCCEEDED(last_hresult_)) {
        state_ = DRV_STOPED;
    }
    return SUCCEEDED(last_hresult_);
}
