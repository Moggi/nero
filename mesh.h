#ifndef MESH_INCLUDED_H
#define MESH_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "obj_loader.h"

struct Vertex
{
public:
	Vertex()
	{
		mNormal = glm::vec3(0);
		mPosition = glm::vec3(0);
		mTextureCoordinates = glm::vec2(0);
	}

	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal)
	{
		mPosition = pos;
		mNormal = normal;
		mTextureCoordinates = texCoord;
	}

	glm::vec3* GetPos() { return &mPosition; }
	glm::vec2* GetTexCoord() { return &mTextureCoordinates; }
	glm::vec3* GetNormal() { return &mNormal; }

	void SetPos(const glm::vec3& pos) {
		mPosition = pos;
	}
	void SetTexCoord(const glm::vec3& texCoords) {
		mTextureCoordinates = texCoords;
	}
	void SetNormal(const glm::vec3& normal) {
		mNormal = normal;
	}

private:
	glm::vec3 mPosition;
	glm::vec2 mTextureCoordinates;
	glm::vec3 mNormal;
};

enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	INDEX_VB
};

//====================================================================================

class Mesh
{
public:
    Mesh(const std::string& fileName);
	// Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);

	virtual ~Mesh();
protected:
	Mesh(){}

	void Init(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void DrawTriangles();
	void DrawTriangleStrip();
private:
	static const unsigned int NUM_BUFFERS = 4;
	void operator=(const Mesh& mesh) {}
	Mesh(const Mesh& mesh) {}

    void InitMesh(const IndexedModel& model);

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	GLsizei m_numIndices;
};

#endif
