#include "EZGL/Mesh.h"
#include "EZGL/Math/Matrix.h"
#include "EZGL/Math/Vector.h"
#include <iostream>

namespace ezgl
{
	size_t Mesh::getByteSize() const
	{
		return this->getPositionsByteSize() + this->getNormalsByteSize() + this->getColorsByteSize();
	}

	size_t Mesh::getPositionsByteSize() const { return this->_positions.size() * sizeof(float); }

	void Mesh::addPoint(const Vector3<float>& vec)
	{
		if (vec.x < _min.x)
			_min.x = vec.x;
		if (vec.x > _max.x)
			_max.x = vec.x;
		if (vec.y < _min.y)
			_min.y = vec.y;
		if (vec.y > _max.y)
			_max.y = vec.y;
		if (vec.z < _min.z)
			_min.z = vec.z;
		if (vec.z > _max.z)
			_max.z = vec.z;
		_center = _min + (_max - _min) / 2.0f;

		this->_positions.push_back(vec.x);
		this->_positions.push_back(vec.y);
		this->_positions.push_back(vec.z);
	}

	void Mesh::addNormal(const Vector3<float>& vec)
	{
		this->_normals.push_back(vec.x);
		this->_normals.push_back(vec.y);
		this->_normals.push_back(vec.z);
	}

	void Mesh::addColor(const Vector3<float>& vec)
	{
		this->_colors.push_back(vec.x);
		this->_colors.push_back(vec.y);
		this->_colors.push_back(vec.z);
	}

	size_t Mesh::getColorsByteSize() const { return this->_colors.size() * sizeof(float); }
	std::vector<float> Mesh::getColors() const { return this->_colors; }

	size_t Mesh::getNormalsByteSize() const { return this->_normals.size() * sizeof(float); }
	std::vector<float> Mesh::getNormals() const { return this->_normals; }

	std::vector<float> Mesh::getPositions() const { return this->_positions; };
	size_t Mesh::getDataTypeSize() const { return sizeof(float); };

	ezgl::Matrix<float, 4, 4> Mesh::getCenterTranslationMatrix() const { return ezgl::translation(_center * -1); }
}
