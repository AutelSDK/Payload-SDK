#ifndef __UAV_UPGRADE_HPP__
#define __UAV_UPGRADE_HPP__
#include <string>
#include <vector>
#include "uav_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    std::string model;
    std::string app_version;
    std::string boot_version;
    std::string hard_version;
}T_ModuleInfo;

typedef enum {
    UAV_UPGRADE_STATE_IDLE = 0,
    UAV_UPGRADE_STATE_DOING = 1,
    UAV_UPGRADE_STATE_FINISH = 2,
    UAV_UPGRADE_STATE_FAILED = 3
}E_UAVUpgradeState;

typedef struct {
    int32_t state;
    int32_t progress;
    int32_t error_code;
    std::string module;
    std::string error_desc;
}T_UAVUpgradeStatus;

typedef struct {
    bool (*getModuleInfo)(T_ModuleInfo &info);
    bool (*upgradeConditionCheck)(void);
    int (*upgradeExcute)(bool force, const std::string &model, const std::string &filename);
    bool (*upgradeQuery)(T_UAVUpgradeStatus &status);
}T_UAVUpgradeHandler;

extern T_UAVReturnCode UAV_Upgrade_Init(T_UAVUpgradeHandler *handler);
extern void UAV_Upgrade_Exit(void);

typedef struct {
    int64_t file_size;
    int32_t package_size;
    std::string file_name;
}T_UAVFileDesc;

typedef enum {
    DIGEST_TYPE_MD5    = 0,
    DIGEST_TYPE_SHA1   = 1,
    DIGEST_TYPE_CRC32  = 2,
    DIGEST_TYPE_SHA224 = 3,
    DIGEST_TYPE_SHA256 = 4,
    DIGEST_TYPE_SHA384 = 5,
    DIGEST_TYPE_SHA512 = 6,
}E_DigestType;

extern T_UAVReturnCode UAV_Upgrade_Firmware_Download(const std::string &model, T_UAVFileDesc &desc);
extern T_UAVReturnCode UAV_Upgrade_Firmware_Request(int64_t offset, int32_t length, const int msec, std::vector<uint8_t> &data);
extern T_UAVReturnCode UAV_Upgrade_Firmware_Verify(const E_DigestType type, const std::string &digest, const int msec);
extern T_UAVReturnCode UAV_Upgrade_Firmware_Finish(const std::string &reason, const int msec);
extern T_UAVReturnCode UAV_Upgrade_Firmware_CRC32(const std::string &filename, uint32_t &crc32);

#ifdef __cplusplus
}
#endif

#endif
