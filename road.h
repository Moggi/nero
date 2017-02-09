#ifndef ROAD_INCLUDED_H
#define ROAD_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "mesh.h"
#include "gameobject.h"

class Road : public Mesh, public GameObject
{
public:

    Road(int,int);
    ~Road();

    void Draw();

private:
    int mRows;
    int mColumns;
    int mNumberIndices;
    int mNumberVertices;

    Vertex * mVertices;
    Uint32 * mIndices;

    void makeRoad(float,float,int);

};



Road::Road(int rows, int columns)
{
    mRows = rows;
    mColumns = columns;

    // http://stackoverflow.com/a/17006081
    int numIndPerRow = mColumns * 2 + 2;
    int numIndDegensReq = (mRows - 1) * 2;

    mNumberIndices = numIndPerRow * mRows + numIndDegensReq;
    mNumberVertices = (mRows+1) * (mColumns+1);

    mVertices = new Vertex[mNumberVertices];
    mIndices = new Uint32[mNumberIndices];

    makeRoad(-mColumns/2.0f,-mRows/2.0f,0);

    Init(mVertices, mNumberVertices, mIndices, mNumberIndices);
}

Road::~Road()
{
    delete [] mVertices;
    delete [] mIndices;
}


void Road::makeRoad(float offsetX, float offsetY, int offsetZ)
{
    int width = mColumns+1;
    int height = mRows+1;

    int i = 0;
    // Set up vertices
    for( int y = 0; y < height; y++ )
    {
        for( int x = 0; x < width; x++ )
        {
            i = y * width + x;
            mVertices[i].GetPos()->x = x+offsetX;
            mVertices[i].GetPos()->z = y+offsetY;
            mVertices[i].GetPos()->y = offsetZ;
            mVertices[i].SetTexCoord(glm::vec2(x*1.0/mColumns, y*1.0));
            mVertices[i].SetNormal(glm::vec3(0, 0, -1));
        }
    }
    // mVertices[0].SetTexCoord(glm::vec2(1, 1));
    // mVertices[height-1].SetTexCoord(glm::vec2(1, 1));
    // mVertices[width-1].SetTexCoord(glm::vec2(1, 1));
    // mVertices[width-1 + height-1].SetTexCoord(glm::vec2(0, 0));

    i = 0;
    // Set up indices
    height--;
    for( int y = 0; y < height; y++ )
    {
        int base = y * width;

        for( int x = 0; x < width; x++ )
        {
            mIndices[i++] = base + x;
            mIndices[i++] = base + width + x;
        }
        // add a degenerate triangle (except in a last row)
        if( y < height - 1 )
        {
            mIndices[i++] = (y + 1) * width + (width - 1);
            mIndices[i++] = (y + 1) * width;
        }
    }

}

void Road::Draw()
{
    GetTexture()->Bind();
	DrawTriangleStrip();
}


#endif
