# Nero
A micro SDL framework for 3D animation

Everything from 2016~2017

## Dependences

- OpenGL 3.3
- C++11
- [SDL2](https://www.libsdl.org/download-2.0.php)
- [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
- [GLM](http://glm.g-truc.net)
- [GLEW](http://glew.sourceforge.net)


## About

This was made on a MacBook Pro (mid 2012)
with an Intel HD 4000

Compiled over macOS Sierra 10.12 (2017) with clang++

__Libraries installed with [HomeBrew](http://brew.sh)__

```sh
brew install sdl2 sdl2_image sdl2_ttf glm glew
```

#### Usage

```sh
chmod +x compile.sh
./compile.sh <filename>
# where `<filename>` can be "main" or "inf"
# main = history mode
# inf = infinity runner mode

# You need to move main.cpp OR inf.cpp from src/ to current dir
# It will not compile any of the two if the other stay in the same dir.

```


## Thanks

- Tim Jones   
SDLTutorials.com   
https://github.com/MetaCipher/the-sdl-engine

- Benny   
https://github.com/BennyQBD/ModernOpenGLTutorial   
https://www.youtube.com/playlist?list=PLEETnX-uPtBUbVOok816vTl1K9vV1GgH5
