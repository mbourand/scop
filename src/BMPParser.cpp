#include "BMPParser.h"
#include <fstream>
#include <stdexcept>
#include "Logger.h"

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

		unsigned int offset = this->_readInt(content, 0xA);
		if (content.size() <= offset)
			throw std::runtime_error("Invalid BMP file: offset too large");

		this->width = this->_readInt(content, 0x12);
		this->height = this->_readInt(content, 0x16);

		this->pixels.resize(this->width * this->height);

		unsigned short bpp = this->_readShort(content, 0x1C);
		if (bpp != 32)
			throw std::runtime_error("Invalid BMP file: only 32 bits per pixel is supported");

		int at = 0;
		for (size_t i = offset; i < content.size(); i += 4)
		{
			if (i + 3 >= content.size())
				throw std::runtime_error("Invalid BMP file: content size is not a multiple of 4");
			this->pixels[at] = this->_readInt(content, i);
			at++;
		}

		if (this->pixels.size() != this->width * this->height)
			throw std::runtime_error("Invalid BMP file: content size is not a multiple of 4");

		Logger::log("BMP Parser", "File parsed -> width: " + std::to_string(this->width) + ", height: " + std::to_string(this->height), Logger::MessageType::Success, Logger::LoggingLevel::Debug);
	}

	unsigned int BMPParser::_readInt(const std::vector<unsigned char>& vec, size_t index) const
	{
		return (vec[index + 3] << 24) + (vec[index + 2] << 16) + (vec[index + 1] << 8) + vec[index + 0];
	}

	unsigned short BMPParser::_readShort(const std::vector<unsigned char>& vec, size_t index) const
	{
		return (vec[index + 1] << 8) + vec[index + 0];
	}

	ezgl::Texture BMPParser::createTexture()
	{
		std::vector<unsigned char> formattedPixels;
		formattedPixels.resize(this->pixels.size() * 3);
		for (int i = 0; i < this->pixels.size(); i++)
		{
			auto pixel = this->pixels[i];
			formattedPixels[i * 3] = (pixel >> 16) & 0xFF;
			formattedPixels[i * 3 + 1] = (pixel >> 8) & 0xFF;
			formattedPixels[i * 3 + 2] =(pixel >> 0) & 0xFF;
		}
		return ezgl::Texture(this->width, this->height, formattedPixels);
	}
}
