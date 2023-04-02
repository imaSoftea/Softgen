#pragma once

#include <GL/glew.h>
#include <iostream>

#define ASSERT(x) if(!(x)) __debugbreak();

//Debug Line (only applies to debug mode)
#ifdef __DEBUG
	#define DebugCall(x) Softgen::ClearError(); x; ASSERT(Softgen::CheckError(#x, __FILE__, __LINE__))
#else
	#define DebugCall(x) x
#endif


namespace Softgen
{
	static void ClearError()
	{
		while (glGetError());
	}

	static bool CheckError(const char* func, const char* file, unsigned int line)
	{
		bool error = false;
		while (GLenum error = glGetError())
		{
			std::cout << "OPENGL ERROR: "
				<< "[" << error << "] " 
				<< func << " " 
				<< file << " " 
				<< "(" << line << ")" 
				<< std::endl;
			return false;
		}
		return true;
	}
}
