/*
*	Softgen
*	Carter Thompson
*
*	Date Started 3/31/2022
*/

#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Helpers/shaderFunc.h"

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

	//Create Window
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

	float pos[6] =
	{
		-0.5, -0.5,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), pos, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	std::string vertexShader = 
		"#version 410 core\n"
		""
		"layout(location = 0) in vec4 position;\n"
		""
		"void main()"
		"{"
		"	gl_Position = position;\n"
		"}";

	std::string fragmentShader =
		"#version 410 core\n"
		""
		"layout(location = 0) out vec4 color;\n"
		""
		"void main()"
		"{"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}";
	
	unsigned int shader = Softgen::CreateShader(vertexShader, fragmentShader);
	glLinkProgram(shader);
	glUseProgram(shader);

	//Loop
	while (!glfwWindowShouldClose(window))
	{
		//Render
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Swap Buffers
		glfwSwapBuffers(window);

		//Poll
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}