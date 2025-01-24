#ifndef __UAV_UPGRADE_H__
#define __UAV_UPGRADE_H__


#ifdef __cplusplus
extern "C" {
#endif
#include "uav_platform.h"

typedef void (*T_UAVVerifyAlgorithmFunc)(uint32_t *crc, uint8_t *data, uint32_t len);

typedef enum _verify_type{
    E_VERIFY_TYPE_MD5 = 0,
    E_VERIFY_TYPE_SHA1= 1,
    E_VERIFY_TYPE_CRC32= 2,
    E_VERIFY_TYPE_SHA224= 3,
    E_VERIFY_TYPE_SHA256= 4,
    E_VERIFY_TYPE_SHA384= 5,
    E_VERIFY_TYPE_SHA512= 8,
} E_UAVVeryfyType;

typedef struct _file_info{
    /** 文件大小，单位byte */
    int64_t file_size;  
    /** model字段*/
    char model[32];
} T_UAVFileInfo;

typedef struct _upgrade
{
    char *model;
    /* 单包传输长度*/
    uint32_t packet_size; 
    /*升级时间，分钟*/
    int32_t upg_time;
    /*升级任务栈大小*/
    uint32_t upg_task_stack_size;       
    /*校验类型*/
    E_UAVVeryfyType verify_type;
    /**
     * @description: 升级开始回调, 用于处理升级开始前准备工作
     *              返回0表示成功，开始升级
     *              返回-1表示失败，退出升级
     * 
     * @param {*}
     * @return {*} 0-成功，-1-失败
    */
    int (*UpgStartCallback)(void);

    /**
     * @description: 升级文件信息处理回调, 用于处理升级文件信息, 通过该回调函数获取升级文件信息
     *               该回调函数返回0表示成功，-1表示失败; 
     *               当返回0时，表示成功，可以进行文件数据获取操作;
     *               当返回-1时，表示失败。
     * 
     * @param {T_UAVFileInfo *} file_info: 升级文件信息
     * @return {*} 0-成功，-1-失败 
    */
    int (*ReadFileInfoCallback)( T_UAVFileInfo *file_info);
    /**
     * @description: 升级请求数据处理回调, 用于处理升级数据, 通过该回调函数将升级数据写入到设备中
     * 
     * @param {uint32_t} offset: 偏移
     * @param {uint32_t} datalen: 数据长度
     * @param {char *} data: 数据
     * @return {*} 0-成功，-1-失败 
    */
    int (*ReadFileDataCallback)(uint32_t offset, uint32_t datalen, char *data);
    
    /**
     * @description: 升级文件校验回调, 用于处理升级文件校验, 将校验结果通过VerifyCodeOut返回
     *               0-成功：校验完成
     *              -1-失败：校验失败
     * @param {T_UAVVerifyAlgorithmFunc} verifyFunc: 校验算法
     * @param {uint32_t  } VerifyCodeIn: PSDK计算的校验结果
     * @param {uint32_t *} VerifyCodeOut: 校验结果
     * @return {*} 0-成功，-1-失败 
    */
    int (*FileVerifyCallback)(T_UAVVerifyAlgorithmFunc verifyFunc, uint32_t VerifyCodeIn, uint32_t *VerifyCodeOut);
    
    /**
     * @description: 升级完成回调, 用于处理升级完成后的操作
     * 
     * @param {*} result: 升级结果 false-失败，true-成功
     * @return {*}
    */
    void (*UpgFinishCallback)(bool result);
    
}T_UAVUpgrade;

T_UAVReturnCode UAV_RegUpgrade(T_UAVUpgrade *upgrade);

#ifdef  __cplusplus
}
#endif

#endif //__UAV_UPGRADATION_H__