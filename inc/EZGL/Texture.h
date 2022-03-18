#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace ezgl
{
	class Texture
	{
	protected:
		GLuint _id;
	public:
		Texture(unsigned int width, unsigned int height, std::vector<unsigned char> pixels);
		Texture(const Texture& other) = default;
		Texture& operator=(const Texture& other) = default;

		void bind();
		void unbind();
	};
}
