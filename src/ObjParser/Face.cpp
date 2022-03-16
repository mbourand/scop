#include "..\..\inc\ObjParser\Face.h"

scop::Face::Face()
{}

scop::Face::Face(const scop::Face& other)
{
	*this = other;
}

scop::Face& scop::Face::operator=(const scop::Face& other)
{
	this->_vertex_idxs = other._vertex_idxs;
	this->_texture_idxs = other._texture_idxs;
	this->_normal_idxs = other._normal_idxs;
	return *this;
}

void scop::Face::addPoint(unsigned int vertex, unsigned int texture, unsigned int normal)
{
	this->_vertex_idxs.push_back(vertex);
	this->_texture_idxs.push_back(texture);
	this->_normal_idxs.push_back(normal);
}

size_t scop::Face::getSize() const
{
	return this->_vertex_idxs.size();
}

int scop::Face::getVertex(size_t index) const
{
	return this->_vertex_idxs[index];
}

int scop::Face::getTexture(size_t index) const
{
	return this->_texture_idxs[index];
}

int scop::Face::getNormal(size_t index) const
{
	return this->_normal_idxs[index];
}
