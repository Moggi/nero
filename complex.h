#ifndef COMPLEX_INCLUDED_H
#define COMPLEX_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "mesh.h"
#include "gameobject.h"

class Complex : public Mesh, public GameObject
{
public:

    Complex(const std::string& fileName);

    void Draw();


private:
    Complex(){}

};

Complex::Complex(const std::string& fileName)
    : Mesh(fileName.c_str()), GameObject()
{
}

void Complex::Draw()
{
    GetTexture()->Bind();
	DrawTriangles();
}


#endif
