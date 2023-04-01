
/*  Softgen
*	Carter Thompson
*
*	Date Started 3/31/2022
*/

#include <GLFW/glfw3.h>

#include <iostream>

int main()
{
	GLFWwindow* window;

	#ifdef __DEBUG
	std::cout << "DEBUG IS ENABLED" << std::endl;
	#endif

	//Initialize the library
	if (!glfwInit())
	{	
		

		return -1;
	}

	return 0;
}