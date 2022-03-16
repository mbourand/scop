#pragma once

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
	#define OS_WINDOWS
#elif defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || defined(__MACH__)
	#define OS_MACOS
#elif defined(__unix__) || defined(__unix) || defined(unix) || defined(__linux__) || defined(__linux)
	#define OS_LINUX
#endif

#include <list>
#include <string>

namespace utils
{
	std::list<std::string> split(const std::string& str, const std::string& charset);
}