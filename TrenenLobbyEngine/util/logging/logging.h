#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger
{
public:
	void Init();
	void Destroy();

	inline static std::shared_ptr<spdlog::logger>& GetLogEngine() { return Log; }

private:
	static std::shared_ptr<spdlog::logger> Log;
	Singleton(Logger);
};

#define TLE_CRITICAL(...)	Logger::GetLogEngine()->critical(__VA_ARGS__)
#define TLE_ERROR(...)		Logger::GetLogEngine()->error(__VA_ARGS__)
#define TLE_WARN(...)		Logger::GetLogEngine()->warn(__VA_ARGS__)
#define TLE_INFO(...)		Logger::GetLogEngine()->info(__VA_ARGS__)
#define TLE_DEBUG(...)		Logger::GetLogEngine()->debug(__VA_ARGS__)
#define TLE_TRACE(...)		Logger::GetLogEngine()->trace(__VA_ARGS__)