#include "Logger.h"

std::string Logger::getMessageTypePrefix(Logger::MessageType type)
{
	switch (type)
	{
	case MessageType::Info:
		return "Info";
	case MessageType::Success:
		return "Success";
	case MessageType::Warning:
		return "Warning";
	case MessageType::Error:
		return "Error";
	}
	throw std::invalid_argument("Logger: Unknown MessageType");
}

bool Logger::log(std::string context, const std::string& message, Logger::MessageType type, Logger::LoggingLevel level)
{
	static Logger::LoggingLevel loggingLevel = Logger::LoggingLevel::Normal;

	if (loggingLevel != Logger::LoggingLevel::Muted && loggingLevel >= level)
	{
		std::cout << "[" << context << "] " << getMessageTypePrefix(type) << ": " << message << std::endl;
		return true;
	}
	return false;
}
