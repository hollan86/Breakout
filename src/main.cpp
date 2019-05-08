#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <game.hpp>
#include <resource_manager.hpp>

using namespace std::chrono;

//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes matrices and clear color
bool initGL();

GLFWwindow* window;
high_resolution_clock::time_point current;
duration<double> time_span;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLuint VaoId;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);


int main()
{
		high_resolution_clock::time_point past = high_resolution_clock::now();
		if ( !init() )
		{	
			printf( "Failed to initialize!\n" );
		}
		else
		{

			// Initialize game
			Breakout.Init();

			// Start Game within Menu State
			Breakout.State = GAME_ACTIVE;

			while ( !glfwWindowShouldClose(window) )
			{

				current = high_resolution_clock::now();
				time_span = duration_cast<duration<double>>(current - past);

				if (time_span.count() <= 1.0 / 30.0)
				{
					continue;
				}
//				processInput(window);

				Breakout.ProcessInput(time_span.count());

				// Update Game state
				Breakout.Update(time_span.count());

				// Render
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				Breakout.Render();
				
				

				auto error = glGetError();
				if ( error != GL_NO_ERROR )
				{
					std::cout << "render stage: error initializing opengl! " << std::endl;

				}
				


				/* swap front and back buffers */
				glfwSwapBuffers(window);

				/* poll for and process events */
				glfwPollEvents();
				past = current;
			}

			ResourceManager::Clear();

			glfwTerminate();
		}

	return 0;
}


bool init()
{

	//Initialization flag
	bool success = true;

	//Initialize SDL
	if ( !glfwInit() )
	{
		// printf( "GLFW could not initialize! SDL Error: %s\n", SDL_GetError() );
		printf( "GLFW could not initialize! GLFW Error: " );
		success = false;
	}
	else
	{
		//Use OpenGL 3.3 -- Make sure you have a graphics card that supports 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		//Create window
		window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
		if ( window == nullptr )
		{
			// printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			printf( "Window could not be created!");
			success = false;
		}
		else
		{
			//Create context
			glfwMakeContextCurrent(window);

			//Registering callbacks
			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

			//NOTE: HANDLE KEYS REMOVED!!!
//			glfwSetKeyCallback(window, handleKeys);
			glfwSetKeyCallback(window, key_callback);

//            glfwSetCursorPosCallback(window, mouse_callback);
//            glfwSetScrollCallback(window, scroll_callback);

            //tell GLFW to capture our mouse
//            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			//Initialize OpenGL
			if ( !initGL() )
			{
				printf( "OUCH Unable to initialize OpenGL!\n" );
				success = false;
			}

		}
	}

	GLuint vao;
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	return success;
}

bool initGL()
{
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return false;
	}

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window,&bufferWidth,&bufferHeight);

//	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0,0,SCREEN_WIDTH*2,SCREEN_HEIGHT*2);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glViewport(0,0,bufferWidth*,bufferWidth*);

	//NOTE! For OSX with retina display, framebuffer size should be set twice as much as window size!
//	glViewport(0,0,SCR_WIDTH*2,SCR_HEIGHT*2);
	//configure global opengl state
	//glEnable(GL_DEPTH_TEST);
	//setting polygon mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return true;
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Breakout.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			Breakout.Keys[key] = GL_FALSE;
	}
}


