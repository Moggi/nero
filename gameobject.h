#ifndef GAMEOBJECT_INCLUDED_H
#define GAMEOBJECT_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "transform.h"
#include "texture.h"

class GameObject
{
public:
    GameObject(){ }

    inline Texture* GetTexture() { return &tex; }
    inline Transform& GetTransform() { return trans; }


protected:

private:

    Transform trans;
    Texture tex;
};



#endif
