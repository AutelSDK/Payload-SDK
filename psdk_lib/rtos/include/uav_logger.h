/*
 * @Author: kevin && R22006/uavrobotics.cn
 * @Date: 2023-10-12 09:47:06
 * @LastEditors: kevin && R22006/uavrobotics.cn
 * @LastEditTime: 2023-10-12 14:32:20
 * @Description: uav logger console interface file.
 * Copyright (c) 2023 Uav Robotics. All rights reserved.
 */
#ifndef _UAV_LOGGER_H_
#define _UAV_LOGGER_H_

#include "uav_platform.h"
extern uint32_t get_tick(void);
#ifdef __cplusplus
extern "C" {
#endif

typedef T_UAVReturnCode(*ConsoleFunc)(const uint8_t *data, uint16_t dataLen);

/**
 * @brief Logger console content.
 */
typedef struct {
    ConsoleFunc func;
    uint8_t consoleLevel;
    bool isSupportColor;
} T_UAVLoggerConsole;

/**
 * @brief Logger console level.
 */
typedef enum {
    UAV_LOGGER_CONSOLE_LOG_LEVEL_ERROR = 0, /*!< Logger console error level. The method and level of the console are
                                                  associated with each other. If the level of the registered console
                                                  method is lower than this level, the level interface will not be
                                                  printed successfully. */
    UAV_LOGGER_CONSOLE_LOG_LEVEL_WARN = 1, /*!< Logger console warning level.The method and level of the console are
                                                    associated with each other. If the level of the registered console
                                                    method is lower than this level, the level interface will not be
                                                    printed successfully. */
    UAV_LOGGER_CONSOLE_LOG_LEVEL_INFO = 2, /*!< Logger console info level. The method and level of the console are
                                                 associated with each other. If the level of the registered console
                                                 method is lower than this level, the level interface will not be
                                                 printed successfully. */
    UAV_LOGGER_CONSOLE_LOG_LEVEL_DEBUG = 3, /*!< Logger console debug level. The method and level of the console are
                                                  associated with each other. If the level of the registered console
                                                  method is lower than this level, the level interface will not be
                                                  printed successfully. */
} E_UAVLoggerConsoleLogLevel;


/**
 * @brief Send the log information to the drone storage.
 * @param fmt: pointer to the format string that needs print out.
 * @return Execution result.
*/
void UAV_Logger_Send(uint8_t level, const char* restrict fmt, ...);


/* Exported functions --------------------------------------------------------*/
/**
 * @brief Add the console function and level for Payload SDK.
 * @note When registering the console, you need to provide the method of the console and the level corresponding to
 * the method. Log levels from high to low are Debug, Info, Warn, and Error, the log function module can print all
 * logs not higher than the specified level. Maximum support for registering eight different console methods at the
 * same time.Before registering the console method, you should test the registration method ensure that all the method
 * are normally. If you registered multiple methods at the same time, all the methods will be print.
 * @param console: pointer to the console function.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Logger_AddConsole(T_UAVLoggerConsole *console);

/**
 * @brief Remove the console function and level for Payload SDK.
 * @param console: pointer to the console function.
 * @return Execution result.
 */
T_UAVReturnCode UAV_Logger_RemoveConsole(T_UAVLoggerConsole *console);
/**
 * @brief Print out the selected level log of the specified format by the registration method.
 * @note The registered method is printed according to the corresponding level. If the level of the console is lower
 * than the level at which the log needs to be printed, it will not be printed successfully.
 * @param fmt: pointer to the format string that needs print out.
 * @param ...: Variable parameters, consistent with the use of the system interface print out.
 */
void UAV_Logger_UserLogOutput(uint8_t level, const char* restrict fmt, ...);


/* Exported constants --------------------------------------------------------*/
#define UAV_LOG_FORMAT(format, ...)      "[%s:%d) "format, __FUNCTION__, __LINE__, ##__VA_ARGS__

#define UAV_LOG_ERROR(format, ...)    \
        UAV_Logger_UserLogOutput(UAV_LOGGER_CONSOLE_LOG_LEVEL_ERROR, UAV_LOG_FORMAT(format, ##__VA_ARGS__))
#define UAV_LOG_WARN(format, ...)     \
        UAV_Logger_UserLogOutput(UAV_LOGGER_CONSOLE_LOG_LEVEL_WARN, UAV_LOG_FORMAT(format, ##__VA_ARGS__))
#define UAV_LOG_INFO(format, ...)     \
        UAV_Logger_UserLogOutput(UAV_LOGGER_CONSOLE_LOG_LEVEL_INFO, UAV_LOG_FORMAT(format, ##__VA_ARGS__))
#define UAV_LOG_DEBUG(format, ...)    \
        UAV_Logger_UserLogOutput(UAV_LOGGER_CONSOLE_LOG_LEVEL_DEBUG, UAV_LOG_FORMAT(format, ##__VA_ARGS__))

                                        
#define RLOG_ERROR(format, ...)    \
        UAV_Logger_Send(UAV_LOGGER_CONSOLE_LOG_LEVEL_ERROR, UAV_LOG_FORMAT(format, ##__VA_ARGS__))
#define RLOG_WARN(format, ...)     \
        UAV_Logger_Send(UAV_LOGGER_CONSOLE_LOG_LEVEL_WARN, UAV_LOG_FORMAT(format, ##__VA_ARGS__))
#define RLOG_INFO(format, ...)     \
        UAV_Logger_Send(UAV_LOGGER_CONSOLE_LOG_LEVEL_INFO, UAV_LOG_FORMAT(format, ##__VA_ARGS__))
#define RLOG_DEBUG(format, ...)    \
        UAV_Logger_Send(UAV_LOGGER_CONSOLE_LOG_LEVEL_DEBUG, UAV_LOG_FORMAT(format, ##__VA_ARGS__))

#define LRLOG_ERROR(format, ...)    \
        do { \
            UAV_LOG_ERROR(format, ##__VA_ARGS__); \
            RLOG_ERROR(format, ##__VA_ARGS__); \
        } while (0)
#define LRLOG_WARN(format, ...)     \
        do { \
            UAV_LOG_WARN(format, ##__VA_ARGS__); \
            RLOG_WARN(format, ##__VA_ARGS__); \
        } while (0)
#define LRLOG_INFO(format, ...)     \
        do { \
            UAV_LOG_INFO(format, ##__VA_ARGS__); \
            RLOG_INFO(format, ##__VA_ARGS__); \
        } while (0)
#define LRLOG_DEBUG(format, ...)    \
        do { \
            UAV_LOG_DEBUG(format, ##__VA_ARGS__); \
            RLOG_DEBUG(format, ##__VA_ARGS__); \
        } while (0)
        



#ifdef __cplusplu
}
#endif
#endif
