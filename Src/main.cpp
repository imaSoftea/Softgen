/*
*	Softgen
*	Carter Thompson
*
*	Date Started 3/31/2022
*/

#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Helpers/shaderFunc.h"
#include "Helpers/errorFunc.h"
#include <glm/gtx/string_cast.hpp>

#ifdef __DEBUG
int main()
#else
#pragma warning(disable: 4099)
int WinMain (_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
#endif
{
	#ifdef __DEBUG
		std::cout << "~ ~ ~ ~ DEBUG IS ENABLED ~ ~ ~ ~" << std::endl;
	#endif

	//Initialize GLFW
	if (!glfwInit())
	{	
		#ifdef __DEBUG
				std::cout << "CANNOT INIT GLFW" << std::endl;
		#endif
		return -1;
	}
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	//Details
	int progWidth = 800;
	int progHeight = 800;
	//Create Window
	GLFWwindow* window = glfwCreateWindow(progWidth, progHeight, "Softgen", NULL, NULL);
	if (!window)
	{
		std::cout << "CANNOT CREATE WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Make Context
	glfwMakeContextCurrent(window);

	//Initialize GLEW
	if (glewInit())
	{
		#ifdef __DEBUG
				std::cout << "CANNOT INIT GLEW" << std::endl;
		#endif
		return -1;
	}
	#ifdef __DEBUG
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	#endif

	//Position of Vertexs
	float pos[24] =
	{
		-0.5f, -0.5f, 0.5f,		//1
		0.5f, -0.5f, 0.5f,		//2
		0.5f, 0.5f, 0.5f,		//3
		-0.5f, 0.5f, 0.5f,		//4

		-0.5f, -0.5f, -0.5f,	//5
		0.5f, -0.5f, -0.5f,		//6
		0.5f, 0.5f, -0.5f,		//7
		-0.5f, 0.5f, -0.5f,		//8
	};

	//Indices of the Shape
	unsigned int indices[36] =
	{
		0, 1, 2,	// front face
		2, 3, 0,

		1, 5, 6,	// right face
		6, 2, 1,

		7, 6, 5,	// back face
		5, 4, 7,

		4, 0, 3,	// left face
		3, 7, 4,

		4, 5, 1,	// bottom face
		1, 0, 4,

		3, 2, 6,	// top face
		6, 7, 3
	};

	//Vertex Buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), pos, GL_STATIC_DRAW);

	//Indice Buffer
	unsigned int ibuffer;
	glGenBuffers(1, &ibuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//Vertex Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	//Shader Setup
	std::string vertexShader = Softgen::ProcessShaderFile("Src/Shaders/D_vert.glsl");
	std::string fragmentShader = Softgen::ProcessShaderFile("Src/Shaders/D_frag.glsl");
		
	unsigned int shader = Softgen::CreateShader(vertexShader, fragmentShader);
	glLinkProgram(shader);
	glUseProgram(shader);

	//Color
	int location = glGetUniformLocation(shader, "u_Color");
	glUniform4f(location, 0.6f, 0.9f, 0.9f, 1.0f);
	
	float colors[24] =
	{
		1.0f, 0.0f, 0.0f,	// Red for the first vertex
		0.0f, 1.0f, 0.0f,	// Green for the second vertex
		0.0f, 0.0f, 1.0f,	// Blue for the third vertex
		0.5f, 0.5, 0.0f,	// Yellow for the fourth vertex

		0.0f, 0.0f, 0.0f,	// Black for the first vertex
		0.5f, 0.0f, 5.0f,	// Purple for the second vertex
		0.0f, 0.5f, 0.5f,	// Cyan for the third vertex
		1.0f, 1.0, 1.0f		// White for the fourth vertex
	};

	unsigned int colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glEnableVertexAttribArray(1);

	//Camera
	glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTar(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDir = glm::normalize(cameraPos - cameraTar);

	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up,cameraDir));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDir, cameraRight));

	//Views
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)progWidth / (float)progHeight, 0.1f, 100.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	DebugCall(glUniformMatrix4fv(glGetUniformLocation(shader, "proj"), 1, GL_FALSE, glm::value_ptr(proj)));
	DebugCall(glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model)));
	DebugCall(glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view)));

	//Zbuffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Loop
	while (!glfwWindowShouldClose(window))
	{
		//Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DebugCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));

		view = glm::rotate(view, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		DebugCall(glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view)));

		//Swap Buffers
		glfwSwapBuffers(window);

		//Poll
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}