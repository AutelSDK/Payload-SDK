#ifndef __UAV_SPDLOG_H__
#define __UAV_SPDLOG_H__

#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>

#include "uav_typedef.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/bundled/printf.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
namespace uav
{

template <typename... Args>
static void print(const spdlog::source_loc &loc, spdlog::level::level_enum lvl, const char *fmt, const Args &... args)
{
    spdlog::log(loc, lvl, fmt::sprintf(fmt, args...).c_str());
}

class logStream : public std::ostringstream
{
public:
    explicit logStream(const spdlog::source_loc &loc, spdlog::level::level_enum lvl) : m_loc(loc), m_lvl(lvl) {}
    ~logStream() { flush(); }
    void flush() { spdlog::log(m_loc, m_lvl, str().c_str()); }
private:
    spdlog::source_loc m_loc;
    spdlog::level::level_enum m_lvl = spdlog::level::info;
};

}

///< use fmt lib, e.g. LOG_INFO("info log, {1}, {2}, {1}", 1, 2)
#define LOG_TRACE(msg,...)      spdlog::log({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::trace, msg, ##__VA_ARGS__)
#define LOG_DEBUG(msg,...)      spdlog::log({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::debug, msg, ##__VA_ARGS__)
#define LOG_INFO(msg,...)       spdlog::log({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::info, msg, ##__VA_ARGS__)
#define LOG_WARN(msg,...)       spdlog::log({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::warn, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg,...)      spdlog::log({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::err, msg, ##__VA_ARGS__)
#define LOG_FATAL(msg,...)      spdlog::log({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::critical, msg, ##__VA_ARGS__)

///< use like printf, e.g. PLOG_INFO("info log, %d-%d", 1, 2)
#define PLOG_TRACE(fmt,...)     uav::print({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::trace, fmt, ##__VA_ARGS__)
#define PLOG_DEBUG(fmt,...)     uav::print({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::debug, fmt, ##__VA_ARGS__)
#define PLOG_INFO(fmt,...)      uav::print({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::info, fmt, ##__VA_ARGS__)
#define PLOG_WARN(fmt,...)      uav::print({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::warn, fmt, ##__VA_ARGS__)
#define PLOG_ERROR(fmt,...)     uav::print({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::err, fmt, ##__VA_ARGS__)
#define PLOG_FATAL(fmt,...)     uav::print({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::critical, fmt, ##__VA_ARGS__)

///< use like stream, e.g. SLOG_INFO() << "warn log: " << 1
#define SLOG_TRACE()            uav::logStream({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::trace)
#define SLOG_DEBUG()            uav::logStream({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::debug)
#define SLOG_INFO()             uav::logStream({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::info)
#define SLOG_WARN()             uav::logStream({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::warn)
#define SLOG_ERROR()            uav::logStream({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::err)
#define SLOG_FATAL()            uav::logStream({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::critical)

template<typename... Args>
std::string format_log(const char *fmt, const Args &... args)
{
    return fmt::sprintf(fmt, args...);
}

#ifdef __cplusplus
extern "C" {
#endif

extern T_UAVReturnCode logger_init(std::string log_profile);

extern void remote_log_write(const std::string &log);

#define RLOG_FILENAME       (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define RLOG_TRACE(format,...) remote_log_write(format_log("[TRACE] %s:%d %s() : " format, RLOG_FILENAME, __LINE__, __FUNCTION__, ##__VA_ARGS__))
#define RLOG_DEBUG(format,...) remote_log_write(format_log("[DEBUG] %s:%d %s() : " format, RLOG_FILENAME, __LINE__, __FUNCTION__, ##__VA_ARGS__))
#define RLOG_INFO(format,...)  remote_log_write(format_log("[INFO] %s:%d %s() : " format, RLOG_FILENAME, __LINE__, __FUNCTION__, ##__VA_ARGS__))
#define RLOG_WARN(format,...)  remote_log_write(format_log("[WARN] %s:%d %s() : " format, RLOG_FILENAME, __LINE__, __FUNCTION__, ##__VA_ARGS__))
#define RLOG_ERROR(format,...) remote_log_write(format_log("[ERROR] %s:%d %s() : " format, RLOG_FILENAME, __LINE__, __FUNCTION__, ##__VA_ARGS__))

#ifdef __cplusplus
}
#endif

#endif
