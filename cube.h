#ifndef CUBE_INCLUDED_H
#define CUBE_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>


class Cube : public Mesh
{
public:

    Cube();
    ~Cube();

    void Draw();

private:
    int mNumberIndices;
    int mNumberVertices;

    Vertex * mVertices;
    Uint32 * mIndices;

    void makeCube(int,int,int);

};


Cube::Cube()
{
    mNumberIndices = 6*6;
    mNumberVertices = 4*6;

    mVertices = new Vertex[mNumberVertices];
    mIndices = new Uint32[mNumberIndices];

    makeCube(0,0,0);

    Init(mVertices, mNumberVertices, mIndices, mNumberIndices);
}

Cube::~Cube()
{
    delete [] mVertices;
    delete [] mIndices;
}


void Cube::makeCube(int offsetX, int offsetY, int offsetZ)
{

    int i=0;
    mVertices[i].SetPos(glm::vec3(-1+offsetX, -1+offsetY, -1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,0));
    mVertices[i++].SetNormal(glm::vec3(0, 0, -1));
    mVertices[i].SetPos(glm::vec3(-1+offsetX, 1+offsetY, -1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,0));
    mVertices[i++].SetNormal(glm::vec3(0, 0, -1));
    mVertices[i].SetPos(glm::vec3(1+offsetX, 1+offsetY, -1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,1));
    mVertices[i++].SetNormal(glm::vec3(0, 0, -1));
    mVertices[i].SetPos(glm::vec3(1+offsetX, -1+offsetY, -1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,1));
    mVertices[i++].SetNormal(glm::vec3(0, 0, -1));

    mVertices[i].SetPos(glm::vec3(-1+offsetX, -1+offsetY, 1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,0));
    mVertices[i++].SetNormal(glm::vec3(0, 0, 1));
    mVertices[i].SetPos(glm::vec3(-1+offsetX, 1+offsetY, 1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,0));
    mVertices[i++].SetNormal(glm::vec3(0, 0, 1));
    mVertices[i].SetPos(glm::vec3(1+offsetX, 1+offsetY, 1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,1));
    mVertices[i++].SetNormal(glm::vec3(0, 0, 1));
    mVertices[i].SetPos(glm::vec3(1+offsetX, -1+offsetY, 1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,1));
    mVertices[i++].SetNormal(glm::vec3(0, 0, 1));

    mVertices[i].SetPos(glm::vec3(-1+offsetX, -1+offsetY, -1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,1));
    mVertices[i++].SetNormal(glm::vec3(0, -1, 0));
    mVertices[i].SetPos(glm::vec3(-1+offsetX, -1+offsetY, 1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,1));
    mVertices[i++].SetNormal(glm::vec3(0, -1, 0));
    mVertices[i].SetPos(glm::vec3(1+offsetX, -1+offsetY, 1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,0));
    mVertices[i++].SetNormal(glm::vec3(0, -1, 0));
    mVertices[i].SetPos(glm::vec3(1+offsetX, -1+offsetY, -1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,0));
    mVertices[i++].SetNormal(glm::vec3(0, -1, 0));

    mVertices[i].SetPos(glm::vec3(-1+offsetX, 1+offsetY, -1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,1));
    mVertices[i++].SetNormal(glm::vec3(0, 1, 0));
    mVertices[i].SetPos(glm::vec3(-1+offsetX, 1+offsetY, 1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,1));
    mVertices[i++].SetNormal(glm::vec3(0, 1, 0));
    mVertices[i].SetPos(glm::vec3(1+offsetX, 1+offsetY, 1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,0));
    mVertices[i++].SetNormal(glm::vec3(0, 1, 0));
    mVertices[i].SetPos(glm::vec3(1+offsetX, 1+offsetY, -1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,0));
    mVertices[i++].SetNormal(glm::vec3(0, 1, 0));

    mVertices[i].SetPos(glm::vec3(-1+offsetX, -1+offsetY, -1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,1));
    mVertices[i++].SetNormal(glm::vec3(-1, 0, 0));
    mVertices[i].SetPos(glm::vec3(-1+offsetX, -1+offsetY, 1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,0));
    mVertices[i++].SetNormal(glm::vec3(-1, 0, 0));
    mVertices[i].SetPos(glm::vec3(-1+offsetX, 1+offsetY, 1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,0));
    mVertices[i++].SetNormal(glm::vec3(-1, 0, 0));
    mVertices[i].SetPos(glm::vec3(-1+offsetX, 1+offsetY, -1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,1));
    mVertices[i++].SetNormal(glm::vec3(-1, 0, 0));

    mVertices[i].SetPos(glm::vec3(1+offsetX, -1+offsetY, -1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,1));
    mVertices[i++].SetNormal(glm::vec3(1, 0, 0));
    mVertices[i].SetPos(glm::vec3(1+offsetX, -1+offsetY, 1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,0));
    mVertices[i++].SetNormal(glm::vec3(1, 0, 0));
    mVertices[i].SetPos(glm::vec3(1+offsetX, 1+offsetY, 1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,0));
    mVertices[i++].SetNormal(glm::vec3(1, 0, 0));
    mVertices[i].SetPos(glm::vec3(1+offsetX, 1+offsetY, -1+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,1));
    mVertices[i++].SetNormal(glm::vec3(1, 0, 0));

    i=0;
	mIndices[i++] = 0;
    mIndices[i++] = 1;
    mIndices[i++] = 2;
    mIndices[i++] = 0;
    mIndices[i++] = 2;
    mIndices[i++] = 3;
    mIndices[i++] = 6;
    mIndices[i++] = 5;
    mIndices[i++] = 4;
    mIndices[i++] = 7;
    mIndices[i++] = 6;
    mIndices[i++] = 4;
    mIndices[i++] = 10;
    mIndices[i++] = 9;
    mIndices[i++] = 8;
    mIndices[i++] = 11;
    mIndices[i++] = 10;
    mIndices[i++] = 8;
    mIndices[i++] = 12;
    mIndices[i++] = 13;
    mIndices[i++] = 14;
    mIndices[i++] = 12;
    mIndices[i++] = 14;
    mIndices[i++] = 15;
    mIndices[i++] = 16;
    mIndices[i++] = 17;
    mIndices[i++] = 18;
    mIndices[i++] = 16;
    mIndices[i++] = 18;
    mIndices[i++] = 19;
    mIndices[i++] = 22;
    mIndices[i++] = 21;
    mIndices[i++] = 20;
    mIndices[i++] = 23;
    mIndices[i++] = 22;
    mIndices[i++] = 20;
}

void Cube::Draw()
{
	DrawTriangles();
}


#endif
