#!/bin/bash
# clang++ -o $1 display.cpp obj_loader.cpp shader.cpp mesh.cpp stb_image.cpp texture.cpp -std=c++11 -L/usr/local/lib -I/usr/local/include -lGLEW -lSDL2 -lSDL2_image -lSDL2_ttf -framework OpenGL "./$1.cpp" && ./$1
if [ -z ${1+x} ]
then
    file="main"
else
    file="$1"
fi
file="bin/${file}"
make -s && [[ -r ./$file ]] && ./$file
