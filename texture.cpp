#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "texture.h"
#include "stb_image.h"
#include <iostream>

void Texture::loadFromText(const std::string & message, const std::string & fontFile,
	SDL_Color color, int fontSize)
{
    if(!TTF_WasInit() && TTF_Init()==-1) {
		std::cerr << "Missing library. Could not initialize TTF module" << std::endl;
        return ;
    }
	//Open the font
	TTF_Font * font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == NULL){
		std::cerr << "Could not open TTF font" << std::endl;
		return ;
	}

	SDL_Surface * surf = TTF_RenderUTF8_Blended(font, message.c_str(), color);
	if (surf == NULL){
		TTF_CloseFont(font);
		std::cerr << "Could not render a text" << std::endl;
		return ;
	}
	SDL_Surface * newSurface =
		SDL_CreateRGBSurface(0, surf->w, surf->h, 32, 0, 0, 0, 0);
	SDL_BlitSurface(surf, 0, newSurface, 0); // Blit onto a purely RGB SurfaceSDL_Surface *image = SDL_CreateRGBSurface(0, LoadedImage->w, LoadedImage->h, 16, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	SDL_LockSurface(newSurface);
	setUpTexture((unsigned char*)newSurface->pixels, surf->w, surf->h);
	SDL_UnlockSurface(newSurface);

	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
}

void Texture::loadFromFile(const std::string& fileName)
{
	int width, height, numComponents;
    unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

    if(data == NULL){
		std::cerr << "Unable to load texture: " << fileName << std::endl;
		return ;
	}

	setUpTexture(data,width,height);
	stbi_image_free(data);
}

void Texture::setUpTexture(unsigned char * data, int width, int height)
{
	glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
