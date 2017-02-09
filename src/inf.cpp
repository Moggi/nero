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
static const int TEXTSIZE = 150;

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

//

GLfloat lambo_speed = 11.0f;
GLfloat overall_speed = 1.0f;
GLfloat changing_road_side = 0.0f;
GLfloat MIN_AC = 0.5f;
GLfloat MAX_AC = 6.0f;
GLfloat road_side = 1.0f;

Uint32 totalTime = 0;

bool isRunning = true;
bool collision = false;
bool restart = false;

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


int main(int argc, char** argv)
{
	std::srand(std::time(0)); // use current time as seed for random generator
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Infinity and Beyond");
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

	SDL_Event e;
	float counter = 0.0f;


// #############################################################################
// #############################################################################
// 		Loading assets
// #############################################################################
// #############################################################################

	Uint32 timer = 0;

	HUD historyGUI(1,2);

	HUD fpsGUI(1,1);

    historyGUI.GetTransform().GetScale()->y /= 4.0f;

    historyGUI.GetTexture()->loadFromText(
		"Do your job",
        "./res/sample.ttf",
        WHITE,
        TEXTSIZE
	);

	timer = SDL_GetTicks();
	while( SDL_GetTicks()-timer <= 3000 )
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

// #############################################################################
// #############################################################################
// 		Do the stuff
// #############################################################################
// #############################################################################

    float distance = 0.0f;
	float total_time = 0.0f;
    int meters = 800;
    const int N = 20;
    int i = 0;
    int j = 0;
    Complex lamborghini("./res/lamborghini/lamborghini.obj");
    Road road01(meters+50,5);
    Road road02(meters+50,5);
    glm::vec3 afterCam;
    Cube cubes[N];

    for(j=0; j<N; ++j){
        cubes[j].GetTexture()->loadFromFile("./res/bricks.jpg");
        cubes[j].GetTransform().GetPos()->y = 0.6f;
    }

	camera.MinimumHeight(3.0f);
	camera.FixedLook(3.0f,6.0f);
	camera.Pitch(0.3f);
	camera.MoveUp(4.5f);
	camera.MoveForward(-12.0f);

    lamborghini.GetTexture()->loadFromFile("./res/lamborghini/lamborghini.png");
    road01.GetTexture()->loadFromFile("./res/asphalt_complete.jpg");
    road02.GetTexture()->loadFromFile("./res/asphalt_complete.jpg");

    fpsGUI.GetTransform().GetScale()->x /= 8.0f;
    fpsGUI.GetTransform().GetScale()->y /= 8.0f;
    fpsGUI.GetTransform().GetScale()->z /= 8.0f;

    fpsGUI.GetTransform().GetPos()->x = 1.0f -
        fpsGUI.GetTransform().GetPos()->x - fpsGUI.GetTransform().GetScale()->x;
    fpsGUI.GetTransform().GetPos()->y = 1.0f -
        fpsGUI.GetTransform().GetPos()->y - fpsGUI.GetTransform().GetScale()->y;

    do
    {
		MIN_AC = 0.6f;
		lambo_speed = 11.0f;

		lamborghini.GetTransform() = Transform();
		lamborghini.GetTransform().GetPos()->x -=1.0f;
		lamborghini.GetTransform().GetPos()->z -=10.0f;
		lamborghini.GetTransform().GetScale()->x /=1.5f;
		lamborghini.GetTransform().GetScale()->y /=1.5f;
		lamborghini.GetTransform().GetScale()->z /=1.5f;

        road01.GetTransform() = Transform();
        road02.GetTransform() = Transform();
    	road01.GetTransform().GetPos()->z += meters/2.0f-5.0f;
        road02.GetTransform().GetPos()->z += meters-5.0f;

    	for(j=0; j<N; ++j){
            cubes[j].GetTransform() = Transform();
    		cubes[j].GetTransform().GetPos()->y = 0.6f;
    		cubes[j].GetTransform().GetPos()->x = (std::rand()%2)? -1.0f : 1.0f;
    		cubes[j].GetTransform().GetPos()->z = (j+1)*30;
			cubes[j].GetTransform().GetScale()->x /= 1.5f;
	        cubes[j].GetTransform().GetScale()->y /= 1.5f;
	        cubes[j].GetTransform().GetScale()->z /= 1.5f;
    	}

        restart = false;
        collision = false;

        road_side = -1.0f;
        overall_speed = 0.1f;

		total_time = SDL_GetTicks();

    	while( isRunning && !collision && !restart )
    	{
    		SDL_PumpEvents();

			overall_speed = overall_speed<MIN_AC?MIN_AC:overall_speed;
			overall_speed = overall_speed>MAX_AC?MAX_AC:overall_speed;

    		clearScreen(display,BLACK);

    		camera.Follow(*(lamborghini.GetTransform().GetPos()));

    		changing_road_side = lamborghini.GetTransform().GetPos()->x + (road_side/lambo_speed) * 2.0f;

    		lamborghini.GetTransform().GetPos()->x = changing_road_side;
    		if(changing_road_side < -1.0f)
    			lamborghini.GetTransform().GetPos()->x = -1.0f;

    		if(changing_road_side > 1.0f)
    			lamborghini.GetTransform().GetPos()->x = 1.0f;

    		shader.Bind();

    		for(i=0; i<N; ++i)
    		{
				cubes[i].GetTransform().GetPos()->z -= counter * lambo_speed*overall_speed;

    			distance = glm::distance(*(cubes[i].GetTransform().GetPos()),
    					*(lamborghini.GetTransform().GetPos()));

    			if( distance<=1.5f )
    				collision = true;

    			afterCam = camera.GetPos();
    			afterCam.z -= 6.0f;
    			afterCam.y -= 1.0f;

    			distance = glm::distance(*(cubes[i].GetTransform().GetPos()),afterCam);

    			if( distance<=5.0f )
    				cubes[i].GetTransform().GetPos()->z += (N+1)*30;

    			shader.Update(cubes[i].GetTransform(), camera);
    			cubes[i].Draw();
    		}

    		shader.Update(lamborghini.GetTransform(), camera);
    		lamborghini.Draw();

			shaderGUI.Bind();

			road01.GetTransform().GetPos()->z -= counter * lambo_speed * overall_speed;
			if( road01.GetTransform().GetPos()->z < -(meters/2.0f - 5.0f) )
				road01.GetTransform().GetPos()->z =  meters/2.0f-5.0f;
    		shaderGUI.Update(road01.GetTransform(), camera);
    		road01.Draw();

			road02.GetTransform().GetPos()->z -= counter * lambo_speed * overall_speed;
			if( road01.GetTransform().GetPos()->z < -(meters/2.0f - 5.0f) )
				road01.GetTransform().GetPos()->z =  meters/2.0f - 5.0f;

            shaderGUI.Update(road02.GetTransform(), camera);
    		road02.Draw();

			totalTime = (SDL_GetTicks()-total_time)/1000;
    		fpsGUI.GetTexture()->loadFromText(
    			std::to_string(totalTime),
    	        "./res/sample.ttf",
    	        WHITE,
    	        TEXTSIZE
    		);

    		shaderGUI.Update(fpsGUI.GetTransform(), cameraGUI);
    		fpsGUI.Draw();

    		display.swapBuffers();
    		counter = fpsTime.deltaTime()/1000.0f;

			MIN_AC += counter * 0.1f;
			MIN_AC = MIN_AC>(MAX_AC-1.0f) ? (MAX_AC-1.0f) : MIN_AC;

			lambo_speed += counter * 0.2f;
			lambo_speed = lambo_speed>15.0f ? 15.0f : lambo_speed;

    		fpsTime.update();
    	}

// #############################################################################
// 		GAME OVER
// #############################################################################

    	historyGUI.GetTexture()->loadFromText(
    		std::to_string(totalTime) + "s ",
            "./res/sample.ttf",
            WHITE,
            TEXTSIZE
    	);

        timer = SDL_GetTicks();
    	while( isRunning && SDL_GetTicks()-timer <= 3000 )
    	{
			if(!restart){
                timer = SDL_GetTicks();
			}
			else{
				historyGUI.GetTexture()->loadFromText(
		    		"GAME OVER",
		            "./res/sample.ttf",
		            WHITE,
		            TEXTSIZE
		    	);
			}

            SDL_PumpEvents();

    		clearScreen(display,BLACK);

    		shaderGUI.Bind();

    		shaderGUI.Update(historyGUI.GetTransform(), cameraGUI);
    		historyGUI.Draw();

    		display.swapBuffers();

    		fpsTime.update();
    	}
    } while(restart);

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
                case 0:		// START
					restart=true;
					break;
			}
			overall_speed = overall_speed<MIN_AC?MIN_AC:overall_speed;
			overall_speed = overall_speed>MAX_AC?MAX_AC:overall_speed;
            return 0;
    }

    return 1;
}
