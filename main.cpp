#include <iostream>
#include <SDL2/SDL.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "time.h"

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 600;

int main(int argc, char** argv)
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL 3D");
	display.setVsync(true);

	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1))
	};

	unsigned int indices[] = {0, 1, 2,
							  0, 2, 3
	                          };

	Camera camera(glm::vec3(0.0f, 0.0f, -5.0f), 70.0f,
			(float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 300.0f);

	Shader shader("./res/basicShader");

	Mesh mesh(vertices, sizeof(vertices)/sizeof(vertices[0]),
			indices, sizeof(indices)/sizeof(indices[0]));

	// Mesh monkey("./res/peixinhow.obj");
	Texture texture("./res/bricks.jpg");
	Transform transform;

	/*
	GLint n, i;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);
	for (i = 0; i < n; i++)
		std::cout << glGetStringi(GL_EXTENSIONS,i) << std::endl;
	*/
	transform.GetScale()->x = transform.GetScale()->x/2;
	transform.GetScale()->y = transform.GetScale()->y/2;
	transform.GetScale()->z = transform.GetScale()->z/2;
	transform.GetRot()->x = glm::radians(-45.0f);
	SDL_Event e;
	bool isRunning = true;
	float counter = 0.0f;

	//The frames per second timer
	Time fpsTime;
	fpsTime.requestFrameRate(60);
	fpsTime.start();
	int fps = 0;

	float sinCounter=0;
	float absSinCounter=0;

	while(isRunning)
	{
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
				isRunning = false;
		}

		display.clear(0.0f, 0.3f, 0.6f, 1.0f);

		sinCounter = sinf(counter);
		absSinCounter = std::abs(sinCounter);

		transform.GetPos()->x = sinCounter;
		// transform.GetRot()->y = counter * 2;
		// transform.GetRot()->z = sinCounter*3;


		shader.Bind();
		texture.Bind();
		shader.Update(transform, camera);
		mesh.Draw();
		// monkey.Draw();

		display.swapBuffers();
		counter += fpsTime.deltaTime()/1000.0f;

		fpsTime.update();
	}

	return 0;
}
