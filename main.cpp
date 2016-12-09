#include <iostream>
#include <SDL2/SDL.h>
#include "display.h"
#include "mesh.h"
#include "plane.h"
#include "cube.h"
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

	Camera camera(glm::vec3(0.0f, 0.0f, -5.0f), 70.0f,
			(float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 300.0f);

	Shader shader("./res/basicShader");

	Plane plane(4,4);
	Cube cube;

	// Mesh monkey("./res/peixinhow.obj");
	Mesh lamborghini("./res/lamborghini/lamborghini.obj");

	Texture texture;
	SDL_Color color = { 255, 255, 255, 0 };

	texture.loadFromFile("./res/lamborghini/lamborghini.png");
	// texture.loadFromFile("./res/bricks.jpg");
	// texture.loadFromText(
	// 	"Game Over",
    //     "./res/sample.ttf",
    //     color,
    //     32
	// );
	Transform transform;

	// transform.GetScale()->x = transform.GetScale()->x/2;
	// transform.GetScale()->y = transform.GetScale()->y/2;
	// transform.GetScale()->z = transform.GetScale()->z/2;
	transform.GetPos()->z = 0;
	transform.GetRot()->x = glm::radians(-20.0f);
	SDL_Event e;
	bool isRunning = true;
	float counter = 0.0f;

	//The frames per second timer
	Time fpsTime;
	fpsTime.showFPS(false);
	fpsTime.requestFrameRate(60);
	fpsTime.start();

	float sinCounter=0;
	float absSinCounter=0;

	float _Value1=1.0f;
	float _Value2=1.0f;
	float _Value3=1.0f;

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

		// transform.GetPos()->x = sinCounter * 0.1f;
		// transform.GetPos()->y = sinCounter * 0.1f;
		// transform.GetPos()->x = counter * 0.1f;
		transform.GetRot()->y = counter * 0.9;
		// transform.GetRot()->z = sinCounter*3;

		_Value1 = absSinCounter * 0.0f + 1.0f;

		shader.Bind();
		texture.Bind();
		shader.Update(transform, camera,_Value1,_Value1,_Value1);

		// cube.Draw();
		lamborghini.Draw();
		// plane.Draw();
		// monkey.Draw();

		display.swapBuffers();
		counter += fpsTime.deltaTime()/1000.0f;

		fpsTime.update();
	}

	return 0;
}
