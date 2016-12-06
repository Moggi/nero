#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture(){}

	void loadFromFile(const std::string&);
	void loadFromText(const std::string&,const std::string&,SDL_Color&,int);

	void Bind();

	virtual ~Texture();
protected:
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}

	void setUpTexture(unsigned char*,int,int);

	GLuint m_texture;
};

#endif
