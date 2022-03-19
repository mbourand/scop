#pragma once

#include <string>
#include <vector>
#include "EZGL/Texture.h"

namespace scop
{
	class BMPParser
	{
	protected:
		unsigned int _readInt(const std::vector<unsigned char>& vec, size_t index) const;
		unsigned short _readShort(const std::vector<unsigned char>& vec, size_t index) const;
	
	public:
		std::vector<unsigned int> pixels;
		unsigned int width, height;
	
		BMPParser();
		
		BMPParser(const std::string& filename);
		void loadFile(const std::string& filename);
		ezgl::Texture createTexture();
	};
}
