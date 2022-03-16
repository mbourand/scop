#pragma once

#include <string>
#include <vector>
#include "Face.h"
#include "../EZGL/Math/Vector.h"
#include "..\..\inc\EZGL\Mesh.h"

namespace scop
{
	class ObjParser
	{
	protected:
		std::vector<ezgl::Vector4<float>> _vertrices;
		std::vector<ezgl::Vector4<float>> _textureVertrices;
		std::vector<ezgl::Vector3<float>> _normalVertrices;
		std::vector<Face> _faces;
	
	private:
		void _parseVertex(std::string line);
		void _parseTextureVertex(std::string line);
		void _parseNormalVertex(std::string line);
		void _parseFace(std::string data);

	public:
		ObjParser() = default;
		ObjParser(const ObjParser& other) = default;
		ObjParser& operator=(const ObjParser& other) = default;

		ObjParser(const std::string& filename);
		
		void loadFile(const std::string& filename);

		std::vector<ezgl::Vector4<float>> getVertrices() const;
		std::vector<ezgl::Vector4<float>> getTextureVertrices() const;
		std::vector<ezgl::Vector3<float>> getNormalVertrices() const;
		std::vector<Face> getFaces() const;

		ezgl::Vector4<float> getVertex(std::size_t i) const;
		ezgl::Vector4<float> getTextureVertex(std::size_t i) const;
		ezgl::Vector3<float> getNormalVertex(std::size_t i) const;
		const Face& getFace(std::size_t i) const;

		ezgl::Mesh createMesh() const;

		void logData() const;
	};
}