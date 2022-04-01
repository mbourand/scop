#include "ObjParser\ObjParser.h"
#include <fstream>
#include "Utils.h"
#include "Logger.h"

namespace scop
{
	void ObjParser::_parseVertex(std::string data)
	{
		auto values = utils::split(data, " ");

		if (values.size() < 3 || values.size() > 4)
			throw std::invalid_argument("ObjParser: Invalid obj file");

		std::vector<float> numberValues;
		for (const auto& value : values)
			numberValues.push_back(std::stof(value));
		if (numberValues.size() < 4)
			numberValues.push_back(1.0f);

		this->_vertrices.push_back(ezgl::Vector4<float>(numberValues[0], numberValues[1], numberValues[2], numberValues[3]));
	}

	void ObjParser::_parseTextureVertex(std::string data)
	{
		auto values = utils::split(data, " ");

		if (values.size() < 2 || values.size() > 4)
			throw std::invalid_argument("ObjParser: Invalid obj file");

		std::vector<float> numberValues;
		for (const auto& value : values)
			numberValues.push_back(std::stof(value));
		while (numberValues.size() < 4)
			numberValues.push_back(0.0f);

		this->_textureVertrices.push_back(ezgl::Vector4<float>(numberValues[0], numberValues[1], numberValues[2], numberValues[3]));
	}

	void ObjParser::_parseNormalVertex(std::string data)
	{
		auto values = utils::split(data, " ");

		if (values.size() != 3)
			throw std::invalid_argument("ObjParser: Invalid obj file");

		std::vector<float> numberValues;
		for (const auto& value : values)
			numberValues.push_back(std::stof(value));

		auto vec = ezgl::Vector3<float>(numberValues[0], numberValues[1], numberValues[2]);
		this->_normalVertrices.push_back(vec);
	}

	void ObjParser::_parseFace(std::string data)
	{
		auto at = data.find("//");
		if (at != std::string::npos)
			data = data.replace(at, at + 2, "/0/");

		auto points = utils::split(data, " ");
		if (points.size() < 3)
			throw std::invalid_argument("ObjParser: Invalid obj file");

		Face face;
		for (const auto& point : points)
		{
			auto coords = utils::split(point, "/");
			if (coords.size() < 1 || coords.size() > 3)
				throw std::invalid_argument("ObjParser: Invalid obj file");

			for (const auto& coord : coords)
			{
				if (!std::isdigit(coord.front()) || !std::isdigit(coord.back()))
					throw std::invalid_argument("ObjParser: Invalid obj file");
			}

			std::vector<int> coordsAsNumbers;
			for (const auto& coord : coords)
				coordsAsNumbers.push_back(std::stoi(coord));
			while (coordsAsNumbers.size() < 3)
				coordsAsNumbers.push_back(0);

			face.addPoint(coordsAsNumbers[0], coordsAsNumbers[1], coordsAsNumbers[2]);
		}

		this->_faces.push_back(face);
	}

	void ObjParser::logData() const
	{
		for (std::size_t i = 0; i < this->_vertrices.size(); i++)
		{
			std::string values = std::to_string(this->_vertrices[i].x) + ", " + std::to_string(this->_vertrices[i].y) + ", " +
				std::to_string(this->_vertrices[i].z) + ", " + std::to_string(this->_vertrices[i].w);
			Logger::log("ObjParser", "Vertex " + std::to_string(i) + ": " + values, Logger::MessageType::Info, Logger::LoggingLevel::Debug);
		}

		for (std::size_t i = 0; i < this->_textureVertrices.size(); i++)
		{
			std::string values = std::to_string(this->_textureVertrices[i].x) + ", " + std::to_string(this->_textureVertrices[i].y) + ", " +
				std::to_string(this->_textureVertrices[i].z) + ", " + std::to_string(this->_textureVertrices[i].w);
			Logger::log("ObjParser", "Texture Vertex " + std::to_string(i) + ": " + values, Logger::MessageType::Info, Logger::LoggingLevel::Debug);
		}

		for (std::size_t i = 0; i < this->_normalVertrices.size(); i++)
		{
			std::string values = std::to_string(this->_normalVertrices[i].x) + ", " + std::to_string(this->_normalVertrices[i].y) + ", " +
				std::to_string(this->_normalVertrices[i].z);
			Logger::log("ObjParser", "Normal Vertex " + std::to_string(i) + ": " + values, Logger::MessageType::Info, Logger::LoggingLevel::Debug);
		}

		for (std::size_t i = 0; i < this->_faces.size(); i++)
		{
			std::string values;
			for (std::size_t j = 0; j < this->_faces[i].getSize(); j++)
			{
				values += std::to_string(this->_faces[i].getVertex(j)) + "/" + std::to_string(this->_faces[i].getTexture(j)) + "/" +
					std::to_string(this->_faces[i].getNormal(j)) + " ";
			}
			Logger::log("ObjParser", "Face: " + values, Logger::MessageType::Info, Logger::LoggingLevel::Debug);
		}
	}

	ObjParser::ObjParser(const std::string& filename)
	{
		this->loadFile(filename);
	}

	void ObjParser::loadFile(const std::string& filename)
	{
		Logger::log("ObjParser", "Loading obj file...", Logger::MessageType::Info);

		std::ifstream file(filename);
		if (!file.is_open() || !file.good())
			throw std::runtime_error("ObjParser: Could not open the file " + filename);

		std::string line;
		int fCount = 0, vCount = 0, vtCount = 0, vnCount = 0;
		while (std::getline(file, line))
		{
			if (line.substr(0, 2) == "v ")
				vCount++;
			if (line.substr(0, 3) == "vn ")
				vnCount++;
			if (line.substr(0, 3) == "vt ")
				vtCount++;
			if (line.substr(0, 2) == "f ")
				fCount++;
		}

		Logger::log("ObjParser", "Finished counting vertrices...", Logger::MessageType::Info, Logger::LoggingLevel::Debug);

		file.clear();
		file.seekg(0, std::ios::beg);

		this->_vertrices.reserve(vCount);
		this->_textureVertrices.reserve(vtCount);
		this->_normalVertrices.reserve(vnCount);
		this->_faces.reserve(fCount);

		while (std::getline(file, line))
		{
			auto comment = line.find("#");
			if (comment != std::string::npos)
				line = line.substr(0, line.find("#"));

			// Vertex
			if (line.substr(0, 2) == "v ")
				this->_parseVertex(line.substr(2));
			// Normal Vertex
			if (line.substr(0, 3) == "vn ")
				this->_parseNormalVertex(line.substr(3));
			// Texture Vertex
			if (line.substr(0, 3) == "vt ")
				this->_parseTextureVertex(line.substr(3));

			// Face
			if (line.substr(0, 2) == "f ")
				this->_parseFace(line.substr(2));
		}

		if (this->_vertrices.size() == 0 || this->_faces.size() == 0)
			throw std::invalid_argument("Invalid obj file");

		Logger::log("ObjParser", "File loaded successfully.", Logger::MessageType::Success);
	}

	// https://stackoverflow.com/questions/23723993/converting-quadriladerals-in-an-obj-file-into-triangles
	ezgl::Mesh ObjParser::createMesh() const
	{
		ezgl::Mesh mesh;

		for (const Face& face : this->getFaces())
		{
			float color = 0.07f + (rand() % 3) / 7.0f;
			if (face.getSize() == 3)
			{
				for (size_t i = 0; i < face.getSize(); i++)
				{
					auto point = this->getVertex(face.getVertex(i) - 1);
					mesh.addPoint(point);
					mesh.addColor(ezgl::Vector3<float>(color, color, color));
					if (face.getNormal(i) > 0)
					{
						auto normal = this->getNormalVertex(face.getNormal(i) - 1);
						mesh.addNormal(normal);
					}
					else
					{
						size_t triangle_begin = i - (i % 3);

						auto p1 = this->getVertex(face.getVertex(triangle_begin) - 1);
						auto p2 = this->getVertex(face.getVertex(triangle_begin + 1) - 1);
						auto p3 = this->getVertex(face.getVertex(triangle_begin + 2) - 1);

						auto u = p2 - p1;
						auto v = p3 - p1;

						auto normal = ezgl::Vector3<float>(	u.y * v.z - u.z * v.y,
															u.z * v.x - u.x * v.z,
															u.x * v.y - u.y * v.x);
						mesh.addNormal(normal.normalize());
					}
				}
			}
			else
			{
				for (size_t i = 1; i < face.getSize() - 1; i++)
				{
					mesh.addPoint(this->getVertex(face.getVertex(0) - 1));
					mesh.addPoint(this->getVertex(face.getVertex(i) - 1));
					mesh.addPoint(this->getVertex(face.getVertex(i + 1) - 1));
					if (face.getNormal(0) > 0 && face.getNormal(i) > 0 && face.getNormal(i + 1) > 0)
					{
						mesh.addNormal(this->getNormalVertex(face.getNormal(0) - 1));
						mesh.addNormal(this->getNormalVertex(face.getNormal(i) - 1));
						mesh.addNormal(this->getNormalVertex(face.getNormal(i + 1) - 1));
						mesh.addColor(ezgl::Vector3<float>(color, color, color));
						mesh.addColor(ezgl::Vector3<float>(color, color, color));
						mesh.addColor(ezgl::Vector3<float>(color, color, color));
					}
					else
					{
						for (int k = 0; k < 3; k++)
						{
							auto p1 = this->getVertex(face.getVertex(0) - 1);
							auto p2 = this->getVertex(face.getVertex(i) - 1);
							auto p3 = this->getVertex(face.getVertex(i + 1) - 1);

							auto u = p2 - p1;
							auto v = p3 - p1;

							auto normal = ezgl::Vector3<float>(u.y * v.z - u.z * v.y,
								u.z * v.x - u.x * v.z,
								u.x * v.y - u.y * v.x);

							mesh.addNormal(normal.normalize());
							mesh.addColor(ezgl::Vector3<float>(color, color, color));
						}
					}
				}
			}
		}

		return mesh;
	}

	std::vector<ezgl::Vector4<float>> ObjParser::getVertrices() const { return this->_vertrices; }
	std::vector<ezgl::Vector4<float>> ObjParser::getTextureVertrices() const { return this->_textureVertrices; }
	std::vector<ezgl::Vector3<float>> ObjParser::getNormalVertrices() const { return this->_normalVertrices; }
	std::vector<Face> ObjParser::getFaces() const { return this->_faces; }

	ezgl::Vector4<float> ObjParser::getVertex(std::size_t i) const { return this->_vertrices[i]; }
	ezgl::Vector4<float> ObjParser::getTextureVertex(std::size_t i) const { return this->_textureVertrices[i]; }
	ezgl::Vector3<float> ObjParser::getNormalVertex(std::size_t i) const { return this->_normalVertrices[i]; }
	const Face& ObjParser::getFace(std::size_t i) const { return this->_faces[i]; }
}
