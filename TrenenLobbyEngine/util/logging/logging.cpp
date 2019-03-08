#include <Windows.h>
#include <iostream>
#include <util/util.h>

Singleton_CPP(Logger);

void Logger::Init()
{
	// Create Console
	AllocConsole();
	auto stream1 = freopen("CONIN$", "r", stdin);
	auto stream2 = freopen("CONOUT$", "w", stdout);
	auto stream3 = freopen("CONOUT$", "w", stderr);
	SetConsoleTitleA("TLE Debug");

	// Init SPDLOG
	spdlog::set_pattern("%^[%T] %n / %t: %v%$");
	Log = spdlog::stdout_color_mt("TLE");
	Log->set_level(spdlog::level::trace);
}

void Logger::Destroy()
{
	FreeConsole();
}
