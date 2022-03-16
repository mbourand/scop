#include "../inc/BMPParser.h"
#include <fstream>
#include <stdexcept>
#include "../inc/Logger.h"

namespace scop
{

	BMPParser::BMPParser()
		: width(0), height(0)
	{}


	BMPParser::BMPParser(const std::string& filename)
	{
		this->loadFile(filename);
	}

	void BMPParser::loadFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::binary);

		if (!file.good() || !file.is_open())
			throw std::runtime_error("Could not open the bmp file");

		std::vector<unsigned char> content(std::istreambuf_iterator<char>(file), {});

		if (content.size() < 0x20)
			throw std::runtime_error("Invalid BMP file: file too small");

		unsigned int offset = this->readInt(content, 0xA);
		if (content.size() <= offset)
			throw std::runtime_error("Invalid BMP file: offset too large");

		this->width = this->readInt(content, 0x12);
		this->height = this->readInt(content, 0x16);

		for (size_t i = offset; i < content.size(); i += 4)
		{
			if (i + 3 >= content.size())
				throw std::runtime_error("Invalid BMP file: content size is not a multiple of 4");
			this->pixels.push_back(this->readInt(content, i));
		}

		Logger::log("BMP Parser", "File parsed -> width: " + std::to_string(this->width) + ", height: " + std::to_string(this->height), Logger::MessageType::Success, Logger::LoggingLevel::Debug);
	}

	unsigned int BMPParser::readInt(const std::vector<unsigned char>& vec, size_t index) const
	{
		return (vec[index + 3] << 24) + (vec[index + 2] << 16) + (vec[index + 1] << 8) + vec[index + 0];
	}
}