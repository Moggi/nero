#ifndef HUD_INCLUDED_H
#define HUD_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "mesh.h"
#include "gameobject.h"

class HUD : public Mesh, public GameObject
{
public:

    HUD(int,int);
    ~HUD();

    void Draw();

private:
    int mWidth;
    int mHeight;
    int mNumberIndices;
    int mNumberVertices;

    Vertex * mVertices;
    Uint32 * mIndices;

    void makeHUD(float,float,float);

};


HUD::HUD(int width, int height)
{
    mWidth = width;
    mHeight = height;

    mNumberIndices = 6;
    mNumberVertices = 4;

    mVertices = new Vertex[mNumberVertices];
    mIndices = new Uint32[mNumberIndices];

    makeHUD(-mWidth/2.0f,-mHeight/2.0f,0.0);
    // makeHUD(0.0f,0.0f,0.0f);

    Init(mVertices, mNumberVertices, mIndices, mNumberIndices);
}

HUD::~HUD()
{
    delete [] mVertices;
    delete [] mIndices;
}

void HUD::makeHUD(float offsetX, float offsetY, float offsetZ)
{
    int i=0;

    mVertices[i].SetPos(glm::vec3(0+offsetX, 0+offsetY, 0+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,0));
    mVertices[i++].SetNormal(glm::vec3(0, 0, -1));

    mVertices[i].SetPos(glm::vec3(mWidth+offsetX, 0+offsetY, 0+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,0));
    mVertices[i++].SetNormal(glm::vec3(0, 0, -1));

    mVertices[i].SetPos(glm::vec3(mWidth+offsetX, mHeight+offsetY, 0+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(1,1));
    mVertices[i++].SetNormal(glm::vec3(0, 0, -1));

    mVertices[i].SetPos(glm::vec3(0+offsetX, mHeight+offsetY, 0+offsetZ));
    mVertices[i].SetTexCoord(glm::vec2(0,1));
    mVertices[i++].SetNormal(glm::vec3(0, 0, -1));

    i=0;
    mIndices[i++] = 0;
    mIndices[i++] = 1;
    mIndices[i++] = 2;
    mIndices[i++] = 0;
    mIndices[i++] = 2;
    mIndices[i++] = 3;
}

void HUD::Draw()
{
    glDisable(GL_DEPTH_TEST);

    GetTexture()->Bind();
	DrawTriangles();

    glEnable(GL_DEPTH_TEST);
}


#endif
