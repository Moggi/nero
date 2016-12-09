#include "display.h"
#include <GL/glew.h>
#include <iostream>

Display::Display(int width, int height, const std::string& title)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 	8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 	8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 	8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 	8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,	32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,	16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	m_window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);

	m_glContext = SDL_GL_CreateContext(m_window);

	// Glew will help to see what the drivers support
	// if not Initialized, better quit
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
    if(res != GLEW_OK)
    {
		std::cerr << "Glew failed to initialize!" << std::endl;
    }
	if(!GLEW_ARB_vertex_array_object)
	{
		std::cout << "ARB_vertex_array_object not available." << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_LINE_SMOOTH);
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);
	glFrontFace(GL_FRONT);
	// glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	// glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Display::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::swapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

void Display::setVsync(bool vsync)
{
	if( vsync && SDL_GL_SetSwapInterval(1) )
	{
		std::cerr << "Can not turn on vsync" << std::endl;
	}
	else if( !vsync && SDL_GL_SetSwapInterval(0) )
	{
		std::cerr << "Can not turn on vsync" << std::endl;
	}
}
