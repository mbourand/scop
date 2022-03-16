#pragma once

#include <iostream>

namespace Logger
{
	enum class LoggingLevel
	{
		Muted, Normal, Verbose, Debug, Error_Only
	};

	enum class MessageType
	{
		Info, Success, Warning, Error
	};

	static std::string getMessageTypePrefix(Logger::MessageType type);

	bool log(std::string context, const std::string& message, Logger::MessageType type = Logger::MessageType::Info, Logger::LoggingLevel level = Logger::LoggingLevel::Normal);
};
