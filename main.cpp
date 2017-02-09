#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "display.h"
#include "complex.h"
#include "plane.h"
#include "cube.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "gameobject.h"
#include "camera.h"
#include "time.h"
#include "road.h"
#include "hud.h"

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 600;
static const int TEXTSIZE = 200;

SDL_Color make_color(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Color c= { r, g, b };
	return c;
}

#define WHITE make_color(255,255,255)
#define RED make_color(255,0,0)
#define BLACK make_color(0,0,0)
#define DARK_GRAY make_color(10,10,10)
#define SOFT_BLUE make_color(0,76.5,178.5)

GLfloat cameraSpeed = 0.07f;
GLfloat lambo_speed = 11.0f;
GLfloat volks_speed = 5.0f;
GLfloat overall_speed = 1.0f;
GLfloat changing_road_side = 0.0f;

Uint32 switch_lookAt= 1;
Uint32 total_models = 4;

GLfloat road_side = 1.0f;

bool isRunning = true;
bool collision = false;

struct Keys {
	float front;
	float right;
	float up;
	float rotRight;
	float rotUp;
};

double dot(glm::vec3 a,glm::vec3 b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void clearScreen(Display &display, SDL_Color color);

int JoyStickFilter(void* userdata, SDL_Event* event);

void moveCamera(Camera & camera, Keys key);


int main(int argc, char** argv)
{
	std::srand(std::time(0)); // use current time as seed for random generator
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL 3.3 3D stuff");
	display.setVsync(true);

	Keys key;
	SDL_AddEventWatch(JoyStickFilter, (void*)&key);

	int id=-15;
    SDL_GameController *controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                id=i;
                break;
            } else {
                fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
                return 0;
            }
        }
    }
    auto name = "";
    if( id != -15 ){
        SDL_GameController *pad = SDL_GameControllerOpen( id );
        SDL_Joystick *joy = SDL_GameControllerGetJoystick( pad );

        int instanceID = SDL_JoystickInstanceID( joy );
        name = SDL_GameControllerName(controller);
    }

	Camera cameraGUI(glm::vec3(0.0f, 0.0f, -5.0f),
				(float)DISPLAY_WIDTH,(float)DISPLAY_HEIGHT);

	Camera camera(glm::vec3(0.0f, 0.0f, -5.0f), 70.0f,
				(float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 300.0f);

	Shader shader("./res/basicShader");
	Shader shaderGUI("./res/guiShader");

	//The frames per second timer
	Time fpsTime;
	fpsTime.showFPS(false);
	// fpsTime.requestFrameRate(60);
	fpsTime.start();
	// display.setVsync(false);

	GLboolean SmoothState;

	SDL_Event e;
	float counter = 0.0f;
	float sinCounter=0;
	float absSinCounter=0;


// #############################################################################
// #############################################################################
// 		Loading assets
// 	part1()
// #############################################################################
// #############################################################################

	Uint32 timer = 0;

	HUD historyGUI(1,2);

	Plane parede(4,7);

	HUD fpsGUI(1,1);

	// historyGUI.GetTransform().GetScale()->x /= 1.5f;
	historyGUI.GetTransform().GetScale()->y /= 4.0f;

	historyGUI.GetTexture()->loadFromText(
		// "Procure o profissional",
		"Alcançe seu objetivo",
        "./res/sample.ttf",
        WHITE,
        TEXTSIZE
	);

	timer = SDL_GetTicks();
	while( SDL_GetTicks()-timer <= 3000 )
	{
		// break;
		while(SDL_PollEvent(&e)) {}
		if(!isRunning)
			return 0;

		clearScreen(display,BLACK);

		shaderGUI.Bind();

		shaderGUI.Update(historyGUI.GetTransform(), cameraGUI);
		historyGUI.Draw();

		display.swapBuffers();

		fpsTime.update();
	}

	// Prepare to part2()

	float distance = 1000.0f;

	camera = Camera(glm::vec3(0.0f, 0.0f, -5.0f), 70.0f,
			(float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 300.0f);

	int meters = 800;
	Road road(meters+50,5);
	road.GetTransform().GetPos()->z += meters/2.0f-5.0f;

	Complex lamborghini("./res/lamborghini/lamborghini.obj");
	Complex monkey("./res/monkey3.obj");
	Complex pokeball("./res/pokeball/poke.obj");

	Complex volkswagen("./res/volkswagen/volkswagen.obj");
	volkswagen.GetTransform().GetPos()->x +=1.0f;
	volkswagen.GetTransform().GetPos()->z -=30.0f;
	volkswagen.GetTransform().GetScale()->x /=1.6f;
	volkswagen.GetTransform().GetScale()->y /=1.6f;
	volkswagen.GetTransform().GetScale()->z /=1.6f;

	volkswagen.GetTexture()->loadFromFile("./res/volkswagen/volkswagen.png");
	lamborghini.GetTexture()->loadFromFile("./res/lamborghini/lamborghini.png");
	monkey.GetTexture()->loadFromFile("./res/bricks.jpg");
	pokeball.GetTexture()->loadFromFile("./res/pokeball/pokeball.png");

	road.GetTexture()->loadFromFile("./res/asphalt_complete.jpg");

// #############################################################################
// #############################################################################
// 		Show how to search for a professional
// 		"Game Over"
// 	part2()
// #############################################################################
// #############################################################################


	fpsGUI.GetTransform().GetScale()->x /= 8.0f;
	fpsGUI.GetTransform().GetScale()->y /= 8.0f;
	fpsGUI.GetTransform().GetScale()->z /= 8.0f;

	fpsGUI.GetTransform().GetPos()->x = 1.0f -
		fpsGUI.GetTransform().GetPos()->x - fpsGUI.GetTransform().GetScale()->x;
	fpsGUI.GetTransform().GetPos()->y = 1.0f -
		fpsGUI.GetTransform().GetPos()->y - fpsGUI.GetTransform().GetScale()->y;

	parede.GetTransform().GetPos()->z += meters - 1.0f;
	parede.GetTransform().GetRot()->x = glm::radians(90.0f);
	// parede.GetTransform().GetRot()->y = glm::radians(180.0f);
	parede.GetTransform().GetPos()->y = 2.0f;
	parede.GetTexture()->loadFromFile("./res/xadrez.png");

	camera.MinimumHeight(3.0f);
	camera.FixedLook(3.0f,6.0f);
	camera.Pitch(0.3f);
	camera.MoveUp(4.5f);
	camera.MoveForward(-12.0f);

	int N = 20;
	Cube cubes[20];

	for(int j=0; j<N; ++j){
		cubes[j].GetTexture()->loadFromFile("./res/bricks.jpg");
		cubes[j].GetTransform().GetPos()->y = 0.6f;
		cubes[j].GetTransform().GetScale()->x /= 1.5f;
		cubes[j].GetTransform().GetScale()->y /= 1.5f;
		cubes[j].GetTransform().GetScale()->z /= 1.5f;
		cubes[j].GetTransform().GetPos()->x = (std::rand()%2)? -1.0f : 1.0f;
		cubes[j].GetTransform().GetPos()->z = (j+1)*11;
	}

	glm::vec3 afterCam;

	// overall_speed+=1.0f;

	while( isRunning && !collision)
	{
		SDL_PumpEvents();

		clearScreen(display,BLACK);

		moveCamera(camera, key);
		camera.Follow(*(volkswagen.GetTransform().GetPos()));

		// camera.LookAt(*(volkswagen.GetTransform().GetPos()));

		changing_road_side = volkswagen.GetTransform().GetPos()->x + road_side/volks_speed;

		volkswagen.GetTransform().GetPos()->x = changing_road_side;
		if(changing_road_side < -1.0f)
			volkswagen.GetTransform().GetPos()->x = -1.0f;

		if(changing_road_side > 1.0f)
			volkswagen.GetTransform().GetPos()->x = 1.0f;

		distance = glm::distance(*(parede.GetTransform().GetPos()),
				*(volkswagen.GetTransform().GetPos()));

		if( distance<=3.0f )
			collision = true;

		shader.Bind();


		for(int i=0; i<N; ++i)
		{
			// std::rand();
			distance = glm::distance(*(cubes[i].GetTransform().GetPos()),
					*(volkswagen.GetTransform().GetPos()));

			if( distance<=1.8f )
				collision = true;

			afterCam = camera.GetPos();
			afterCam.z -= 6.0f;
			afterCam.y -= 1.0f;

			distance = glm::distance(*(cubes[i].GetTransform().GetPos()),afterCam);

			if( distance<=5.0f )
				cubes[i].GetTransform().GetPos()->z += 220.0f;

			shader.Update(cubes[i].GetTransform(), camera);
			cubes[i].Draw();
		}

		volkswagen.GetTransform().GetPos()->z += counter * volks_speed*overall_speed;
		shader.Update(volkswagen.GetTransform(), camera);
		volkswagen.Draw();

		shader.Update(road.GetTransform(), camera);
		road.Draw();

		shaderGUI.Bind();
		shaderGUI.Update(parede.GetTransform(), camera);
		parede.Draw();

		fpsGUI.GetTexture()->loadFromText(
			// "Procure o profissional",
			std::to_string((int)fpsTime.fps()) + " fps",
	        "./res/sample.ttf",
	        WHITE,
	        TEXTSIZE
		);

		glGetBooleanv(GL_LINE_SMOOTH,&SmoothState);

		glDisable(GL_LINE_SMOOTH);
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		shaderGUI.Update(fpsGUI.GetTransform(), cameraGUI);
		fpsGUI.Draw();

		if( SmoothState == GL_TRUE )
			glEnable(GL_LINE_SMOOTH);
		glPolygonMode( GL_FRONT_AND_BACK, SmoothState==GL_FALSE?GL_FILL:GL_LINE );

		display.swapBuffers();
		counter = fpsTime.deltaTime()/1000.0f;

		fpsTime.update();
	}

	// isRunning = true;
	counter = 0.0f;
	// road.GetTransform().GetPos()->z += meters/2.0f-10.0f;

	camera = Camera(glm::vec3(0.0f, 0.0f, -5.0f), 70.0f,
			(float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 300.0f);

// #############################################################################
// #############################################################################
// 		Intermission: could be easier
// 	part3()
// #############################################################################
// #############################################################################

	historyGUI.GetTexture()->loadFromText(
		// "Poderia ser mais facil",
		"Poderia ser mais rápido",
        "./res/sample.ttf",
        WHITE,
        TEXTSIZE
	);

	timer = SDL_GetTicks();
	while( SDL_GetTicks()-timer <= 5000 )
	{
		SDL_PumpEvents();
		if(!isRunning)
			return 0;

		clearScreen(display,BLACK);

		shaderGUI.Bind();

		shaderGUI.Update(historyGUI.GetTransform(), cameraGUI);
		historyGUI.Draw();

		display.swapBuffers();

		fpsTime.update();
	}

	lamborghini.GetTransform() = Transform();
	volkswagen.GetTransform() = Transform();
	// road.GetTransform() = Transform();
	// road.GetTransform().GetPos()->z = meters/2.0f-20.0f;

	road_side = -1.0f;
	lamborghini.GetTransform().GetPos()->x -=1.0f;
	// lamborghini.GetTransform().GetPos()->z -= 10.0f;
	lamborghini.GetTransform().GetScale()->x /=1.5f;
	lamborghini.GetTransform().GetScale()->y /=1.5f;
	lamborghini.GetTransform().GetScale()->z /=1.5f;


// #############################################################################
// #############################################################################
// 		Show how to search with servicili
// 	part4()
// #############################################################################
// #############################################################################

	camera = Camera(glm::vec3(0.0f, 0.0f, -5.0f), 70.0f,
			(float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 300.0f);

	// lamborghini.GetTransform().GetPos()->x +=1.0f;
	// lamborghini.GetTransform().GetPos()->z -=10.0f;

	camera.FixedLook(4.0f,6.0f);
	camera.MinimumHeight(3.0f);
	camera.Pitch(0.3f);
	camera.MoveUp(5.5f);
	camera.MoveForward(-12.0f);

	for(int j=0; j<N/2; ++j){
		cubes[j].GetTexture()->loadFromFile("./res/bricks.jpg");
		cubes[j].GetTransform().GetPos()->y = 0.6f;
		cubes[j].GetTransform().GetPos()->x = (std::rand()%2)? -1.0f : 1.0f;
		cubes[j].GetTransform().GetPos()->z = (j+1)*30;
	}

	camera.Follow(*(lamborghini.GetTransform().GetPos()));

	overall_speed=1.0f;

	collision = false;

	while( isRunning && !collision)
	{
		SDL_PumpEvents();

		clearScreen(display,BLACK);

		moveCamera(camera, key);
		camera.Follow(*(lamborghini.GetTransform().GetPos()));

		// camera.LookAt(*(volkswagen.GetTransform().GetPos()));

		changing_road_side = lamborghini.GetTransform().GetPos()->x + road_side/(lambo_speed-volks_speed);

		lamborghini.GetTransform().GetPos()->x = changing_road_side;
		if(changing_road_side < -1.0f)
			lamborghini.GetTransform().GetPos()->x = -1.0f;

		if(changing_road_side > 1.0f)
			lamborghini.GetTransform().GetPos()->x = 1.0f;

		distance = glm::distance(*(parede.GetTransform().GetPos()),
				*(lamborghini.GetTransform().GetPos()));

		if( distance<=3.0f )
			collision = true;

		shader.Bind();

		for(int i=0; i<N/2; ++i)
		{
			// std::rand();
			distance = glm::distance(*(cubes[i].GetTransform().GetPos()),
					*(lamborghini.GetTransform().GetPos()));

			if( distance<=1.8f )
				collision = true;

			afterCam = camera.GetPos();
			afterCam.z -= 6.0f;
			afterCam.y -= 1.0f;

			distance = glm::distance(*(cubes[i].GetTransform().GetPos()),afterCam);

			if( distance<=6.0f )
				cubes[i].GetTransform().GetPos()->z += 300.0f;

			shader.Update(cubes[i].GetTransform(), camera);
			cubes[i].Draw();
		}

		lamborghini.GetTransform().GetPos()->z += counter * lambo_speed*overall_speed;
		shader.Update(lamborghini.GetTransform(), camera);
		lamborghini.Draw();

		shader.Update(road.GetTransform(), camera);
		road.Draw();

		shaderGUI.Bind();
		shaderGUI.Update(parede.GetTransform(), camera);
		parede.Draw();

		fpsGUI.GetTexture()->loadFromText(
			// "Procure o profissional",
			std::to_string((int)fpsTime.fps()) + " fps",
	        "./res/sample.ttf",
	        WHITE,
	        TEXTSIZE
		);

		glGetBooleanv(GL_LINE_SMOOTH,&SmoothState);

		glDisable(GL_LINE_SMOOTH);
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		shaderGUI.Update(fpsGUI.GetTransform(), cameraGUI);
		fpsGUI.Draw();

		if( SmoothState == GL_TRUE )
			glEnable(GL_LINE_SMOOTH);
		glPolygonMode( GL_FRONT_AND_BACK, SmoothState==GL_FALSE?GL_FILL:GL_LINE );

		display.swapBuffers();
		counter = fpsTime.deltaTime()/1000.0f;

		fpsTime.update();
	}

	// isRunning = true;
	counter = 0.0f;

	camera = Camera(glm::vec3(0.0f, 0.0f, -5.0f), 70.0f,
			(float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 300.0f);

// #############################################################################
// #############################################################################
// 		Intermission: This is how we do it
// 	part5()
// #############################################################################
// #############################################################################

	historyGUI.GetTexture()->loadFromText(
		"Viu como é fácil",
		// "Viu como e facil",
		"./res/sample.ttf",
		WHITE,
		TEXTSIZE
	);

	timer = SDL_GetTicks();
	while( SDL_GetTicks()-timer <= 5000 )
	{
		// break;
		while(SDL_PollEvent(&e)) {}
		if(!isRunning)
			return 0;

		clearScreen(display,BLACK);

		shaderGUI.Bind();

		shaderGUI.Update(historyGUI.GetTransform(), cameraGUI);
		historyGUI.Draw();

		display.swapBuffers();

		fpsTime.update();
	}

	camera = Camera(glm::vec3(0.0f, 0.0f, -5.0f), 70.0f,
			(float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 300.0f);

	lamborghini.GetTransform() = Transform();
	volkswagen.GetTransform() = Transform();
	monkey.GetTransform() = Transform();

	volkswagen.GetTransform().GetPos()->x += 6.0f;
	monkey.GetTransform().GetPos()->x -= 6.0f;
	pokeball.GetTransform().GetPos()->x -= 10.0f;

	volkswagen.GetTransform().GetPos()->y -= 0.2f;

	volkswagen.GetTransform().GetScale()->x /=1.3f;
	volkswagen.GetTransform().GetScale()->y /=1.3f;
	volkswagen.GetTransform().GetScale()->z /=1.3f;

	monkey.GetTransform().GetScale()->x /=1.6f;
	monkey.GetTransform().GetScale()->y /=1.6f;
	monkey.GetTransform().GetScale()->z /=1.6f;

	pokeball.GetTransform().GetScale()->x /=1.6f;
	pokeball.GetTransform().GetScale()->y /=1.6f;
	pokeball.GetTransform().GetScale()->z /=1.6f;


// #############################################################################
// #############################################################################
// 		Set a free camera over servicili car
// 	part6()
// #############################################################################
// #############################################################################

	// camera.Lock(true);
	camera.MinimumHeight(0.5f);
	camera.FixedLook(4.0f,4.0f);
	camera.MoveForward(9.9f);
	camera.RotateY(glm::radians(180.0f));
	camera.Pitch(0.4f);
	camera.MoveUp(1.3f);
	overall_speed=1.0f;

	while(isRunning)
	{
		while(SDL_PollEvent(&e)) {}
		clearScreen(display,SOFT_BLUE);

		camera.MoveForward(-cameraSpeed*overall_speed*key.front);
		camera.MoveRight(-cameraSpeed*overall_speed*key.right);
		camera.Pitch(cameraSpeed*key.rotUp);
		camera.RotateY(-cameraSpeed*key.rotRight);
		camera.MoveUp(cameraSpeed*key.up);

		switch(switch_lookAt)
		{
			case 0:
				camera.FixedLook(4.0f,4.0f);
				camera.LookAt(*(volkswagen.GetTransform()).GetPos());
				break;
			case 1:
				camera.FixedLook(4.0f,4.0f);
				camera.LookAt(*(lamborghini.GetTransform()).GetPos());
				break;
			case 2:
				camera.FixedLook(2.0f,2.0f);
				camera.LookAt(*(monkey.GetTransform()).GetPos());
				break;
			case 3:
				camera.FixedLook(1.5f,1.5f);
				camera.LookAt(*(pokeball.GetTransform()).GetPos());
				break;
		}

		shader.Bind();

		shader.Update(lamborghini.GetTransform(), camera);
		lamborghini.Draw();

		shader.Update(volkswagen.GetTransform(), camera);
		volkswagen.Draw();

		shader.Update(monkey.GetTransform(), camera);
		monkey.Draw();

		shader.Update(pokeball.GetTransform(), camera);
		pokeball.Draw();

		shaderGUI.Bind();

		fpsGUI.GetTexture()->loadFromText(
			// "Procure o profissional",
			std::to_string((int)fpsTime.fps()) + " fps",
	        "./res/sample.ttf",
	        WHITE,
	        TEXTSIZE
		);

		glGetBooleanv(GL_LINE_SMOOTH,&SmoothState);

		glDisable(GL_LINE_SMOOTH);
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		shaderGUI.Update(fpsGUI.GetTransform(), cameraGUI);
		fpsGUI.Draw();

		if( SmoothState == GL_TRUE )
			glEnable(GL_LINE_SMOOTH);
		glPolygonMode( GL_FRONT_AND_BACK, SmoothState==GL_FALSE?GL_FILL:GL_LINE );

		display.swapBuffers();
		counter += fpsTime.deltaTime()/1000.0f;

		fpsTime.update();
	}

	return 0;
}

void clearScreen(Display &display, SDL_Color color)
{
	display.clear(color.r/255.0,color.g/255.0,color.b/255.0,1.0f);
}


int JoyStickFilter(void* userdata, SDL_Event* event)
{
	Keys * key = (Keys*)userdata;

	float axisValue = event->caxis.value/32767.0;

	if(axisValue <= 0.1 && axisValue >= -0.1)
		axisValue = 0.0f;

    switch (event->type)
    {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			axisValue = 1.0f;
			switch (event->key.keysym.sym) {
				case SDLK_w:
					key->front = -axisValue*overall_speed;
					break;
				case SDLK_s:
					key->front = axisValue*overall_speed;
					break;
				case SDLK_d:
					key->right = axisValue;
					break;
				case SDLK_a:
					key->right = -axisValue;
					break;
				case SDLK_q:
				case SDLK_LEFT:
					key->rotRight = -axisValue;
					break;
				case SDLK_e:
				case SDLK_RIGHT:
					key->rotRight = axisValue;
					break;
				case SDLK_DOWN:
					key->rotUp = -axisValue;
					break;
				case SDLK_UP:
					key->rotUp = axisValue;
					break;
				default:
					break;
			}
			break;
		case SDL_KEYUP:
			axisValue = 0.0f;
			switch (event->key.keysym.sym) {
				case SDLK_w:
					key->front = axisValue;
					break;
				case SDLK_s:
					key->front = -axisValue;
					break;
				case SDLK_d:
					key->right = axisValue;
					break;
				case SDLK_a:
					key->right = -axisValue;
					break;
				case SDLK_q:
				case SDLK_LEFT:
					key->rotRight = -axisValue;
					break;
				case SDLK_e:
				case SDLK_RIGHT:
					key->rotRight = axisValue;
					break;
				case SDLK_DOWN:
					key->rotUp = -axisValue;
					break;
				case SDLK_UP:
					key->rotUp = axisValue;
					break;
				default:
					break;
			}
			break;
        case SDL_CONTROLLERAXISMOTION:

			switch (event->caxis.axis) {
				case SDL_CONTROLLER_AXIS_RIGHTX:
					key->rotRight = axisValue;
					break;
				case SDL_CONTROLLER_AXIS_RIGHTY:
					key->rotUp = axisValue;
					break;
				case SDL_CONTROLLER_AXIS_LEFTX:
					key->right = axisValue;
					break;
				case SDL_CONTROLLER_AXIS_LEFTY:
					key->front = axisValue;
					break;
				case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
					key->up = -axisValue;
					break;
				case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
					key->up = axisValue;
					break;
			}
            return 0;
		case SDL_CONTROLLERBUTTONUP:
			break;
        case SDL_CONTROLLERBUTTONDOWN:
			switch (event->cbutton.button) {
				case 9:		// L1
					overall_speed -= 0.8f;
					break;
				case 10:	// L2
					overall_speed += 0.8f;
					break;
				case 13:	// RIGHT
					road_side=1.0f;
					break;
				case 14:	// LEFT
					road_side=-1.0f;
					break;
				case 6:		// START
					isRunning=false;
					break;
				case 3:		// TRIANGLE
					glEnable(GL_LINE_SMOOTH);
					glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					break;
				case 2:		// SQUARE
					glDisable(GL_LINE_SMOOTH);
					glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
					break;
				case 0:		// CROSS
					switch_lookAt = (switch_lookAt+1)%total_models;
					break;
			}
			overall_speed = overall_speed<0.5f?0.5f:overall_speed;
			overall_speed = overall_speed>5.5f?5.5f:overall_speed;
            return 0;
    }

    return 1;
}


void moveCamera(Camera & camera, Keys key)
{
	// camera.MoveForward(-cameraSpeed*key.front);
	camera.MoveRight(-cameraSpeed*key.right);
	camera.Pitch(cameraSpeed*key.rotUp);
	// camera.RotateY(-cameraSpeed*key.rotRight);
	// camera.MoveUp(cameraSpeed*key.up);
}
