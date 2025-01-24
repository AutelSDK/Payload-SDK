#include <string>
#include <thread>
#include <atomic>
#include "uav_core.h"
#include "uav_logger.h"
#include "uav_upgrade.h"
#include "uav_platform.h"
#include "uav_sdk_app_info.h"

#define CHECK_TEST(str, iRet)    if(iRet != UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS)    \
                            {                                                   \
                                LOG_ERROR("{} failed",str);                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                                LOG_INFO("{} success",str);                 \
                            }


static std::string s_model("tracer");
static std::atomic_bool s_upgrade_flag(false);

static bool getModuleInfo(T_ModuleInfo &info)
{
    info.model = "tracer";
    info.app_version = "V1.0.0";
    info.boot_version = "V1.0.0";
    info.hard_version = "V1.0.0";
    LOG_INFO("getModuleInfo");
    return true;
}

static bool upgradeConditionCheck(void)
{
    LOG_INFO("upgradeConditionCheck");
    return true;
}

static int upgradeExcute(bool force, const std::string &model, const std::string &filename)
{
    s_upgrade_flag.store(true);
    LOG_INFO("force: {}, model: {}, filename: {}", force, model.c_str(), filename.c_str());
    return 10;
}

static bool upgradeQuery(T_UAVUpgradeStatus &status)
{
    status.state = 1;
    status.progress = 10;
    status.error_code = 0;
    status.module = "tracer";
    return true;
}

static T_UAVUpgradeHandler g_uavUpgradeHandler = {
    .getModuleInfo = getModuleInfo,
    .upgradeConditionCheck = upgradeConditionCheck,
    .upgradeExcute = upgradeExcute,
    .upgradeQuery = upgradeQuery,
};

static void upgrade_routine(void)
{
    T_UAVFileDesc desc;

    // wait register success
    if(false == wait_register_ready(100)){
        LOG_ERROR("waitHandShakeRegister failed");
        return;
    }

    std::vector<uint8_t> data;

    {
        LOG_INFO("测试 UAV_Upgrade_Init 函数");
        // 函数描述：初始化升级模块。
        // 入参：T_UAVUpgradeHandler *handler - 升级模块的回调函数。
        // 返回值：T_UAVReturnCode - 返回码。
        int iRet = UAV_Upgrade_Init(&g_uavUpgradeHandler);
        CHECK_TEST("UAV_Upgrade_Init iRet:{}",iRet);
    }

    {
        LOG_INFO("测试 UAV_Upgrade_Firmware_Download 函数");
        // 函数描述：下载固件。
        // 入参：const std::string &model - 模块名称。
        // 入参：T_UAVFileDesc &desc - 文件描述。
        // 返回值：T_UAVReturnCode - 返回码。
        int iRet = UAV_Upgrade_Firmware_Download("unitTest", desc);
        CHECK_TEST("UAV_Upgrade_Firmware_Download iRet:{}",iRet);
        if (UAV_ERROR_SYSTEM_MODULE_CODE_SUCCESS == iRet)
        {
            LOG_INFO("测试 UAV_Upgrade_Firmware_Request 函数");
            // 函数描述：请求固件。
            // 入参：int64_t offset - 偏移量。
            // 入参：int32_t length - 长度。
            // 入参：const int msec - 超时时间。
            // 入参：std::vector<uint8_t> &data - 数据。
            // 返回值：T_UAVReturnCode - 返回码。

            int64_t read_len = 0;
            std::vector<uint8_t> data;
            int64_t file_size = desc.file_size;
            LOG_INFO("file name: {}, size: {}, package size: {}", desc.file_name, desc.file_size, desc.package_size);
            while(file_size > 0)
            {
                iRet = UAV_Upgrade_Firmware_Request(read_len, desc.package_size, 200, data);
                CHECK_TEST("UAV_Upgrade_Firmware_Request iRet:{}",iRet);
                read_len += data.size();
                file_size -= data.size();
                LOG_INFO("download data size: {}", data.size());
            }
            //对拿到的数据进行校验，并填入9C8E5063位置
            iRet = UAV_Upgrade_Firmware_Verify(DIGEST_TYPE_CRC32, "9C8E5063", 1000);
            CHECK_TEST("UAV_Upgrade_Firmware_Verify iRet:{}",iRet);
            iRet = UAV_Upgrade_Firmware_Finish("seccuss", 50);
            CHECK_TEST("UAV_Upgrade_Firmware_Finish iRet:{}",iRet);
        }
    }

    // {
    //     LOG_INFO("测试 UAV_Upgrade_Firmware_Verify 函数");
    //     // 函数描述：验证固件。
    //     // 入参：const E_DigestType type - 摘要类型。
    //     // 入参：const std::string &digest - 摘要。
    //     // 入参：const int msec - 超时时间。
    //     // 返回值：T_UAVReturnCode - 返回码。
    //     int iRet = UAV_Upgrade_Firmware_Verify(DIGEST_TYPE_CRC32, "9C8E5063", 1000);
    //     CHECK_TEST("UAV_Upgrade_Firmware_Verify iRet:{}",iRet);
    // }

    // {
    //     LOG_INFO("测试 UAV_Upgrade_Firmware_Finish 函数");
    //     // 函数描述：完成固件升级。
    //     // 入参：const std::string &reason - 原因。
    //     // 入参：const int msec - 超时时间。
    //     // 返回值：T_UAVReturnCode - 返回码。
    //     int iRet = UAV_Upgrade_Firmware_Finish("testFinish", 1000);
    //     CHECK_TEST("UAV_Upgrade_Firmware_Finish iRet:{}",iRet);
    // }

    {
        LOG_INFO("测试 UAV_Upgrade_Exit 函数");
        // 函数描述：退出升级模块。
        UAV_Upgrade_Exit();
        CHECK_TEST("UAV_Upgrade_Exit iRet:{}",0);
    }
}

int main(int argc, char *argv[])
{
    T_AUserInfo usrInfo;
    logger_init(std::string(argv[1]));
    if(false == uav_sdk_app_info_init(&usrInfo)) {
        LOG_ERROR("uav_sdk_app_info_init failed");
        return -1;
    }
    UAV_Core_Init(&usrInfo);
    UAV_Core_SetAlias("tracer");
    // UAV_Uart_Init("/dev/pts/20",115200);
    UAV_Network_Init(LOCALHOST_ETHERNET_TEST);

    std::thread upgrade(upgrade_routine);
    upgrade.detach();

    return UAV_Core_ApplicationStart();
}
