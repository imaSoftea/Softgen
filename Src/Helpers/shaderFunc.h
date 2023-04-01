#pragma once

#include <fstream>
#include <string>

namespace Softgen
{
	static unsigned int CompileShader(const std::string& source, unsigned int type)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);
		
		#ifdef __DEBUG
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)calloc(length, sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);

			std::cout << "FAILED TO COMPILE SHADER" << std::endl;
			std::cout << "Type: " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << std::endl;
			std::cout << message << std::endl;

			free(message);
			glDeleteShader(id);
			return 0;
		}
		#endif

		return id;
	}

	static int CreateShader(const std::string& vertexShader, const std::string fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vertex = CompileShader(vertexShader, GL_VERTEX_SHADER);
		unsigned int fragment = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return program;
	}

	static std::string ProcessShaderFile(const std::string& filename)
	{
		std::ifstream file(filename);

		#ifdef __DEBUG
		if (!file.is_open()) {
			std::cerr << "ERROR: cannot open file " << filename << std::endl;
			return "";
		}
		#endif

		std::string content((std::istreambuf_iterator<char>(file)),
			(std::istreambuf_iterator<char>()));

		file.close();

		return content;
	}
}

