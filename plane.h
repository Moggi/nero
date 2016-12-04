#ifndef PLANE_INCLUDED_H
#define PLANE_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>


class Plane : public Mesh
{
public:

    Plane(int,int);
    ~Plane();

    void Draw();

private:
    int mRows;
    int mColumns;
    int mNumberIndices;
    int mNumberVertices;

    Vertex * mVertices;
    Uint32 * mIndices;

    void makePlane(int,int,int);

};



Plane::Plane(int rows, int columns)
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

    makePlane(-mColumns/2,-mRows/2,0);

    Init(mVertices, mNumberVertices, mIndices, mNumberIndices);
}

Plane::~Plane()
{
    delete [] mVertices;
    delete [] mIndices;
}


void Plane::makePlane(int offsetX, int offsetY, int offsetZ)
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
            mVertices[i].GetPos()->y = y+offsetY;
            mVertices[i].GetPos()->z = offsetZ;
            mVertices[i].SetNormal(glm::vec3(0, 0, -1));
        }
    }

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

void Plane::Draw()
{
	DrawTriangleStrip();
}


#endif
