#pragma once

#include <vector>

namespace scop
{
	class Face
	{
	protected:
		std::vector<int> _vertex_idxs;
		std::vector<int> _texture_idxs;
		std::vector<int> _normal_idxs;

	public:
		Face();
		Face(const Face& other);
		Face& operator=(const Face& other);

		void addPoint(unsigned int vertex, unsigned int texture, unsigned int normal);

		size_t getSize() const;
		int getVertex(size_t index) const;
		int getTexture(size_t index) const;
		int getNormal(size_t index) const;
	};
}