//
// Created by Carter Thompson on 9/13/23.
//

#include "ShaderParser.h"

/* Static Initializers */
const std::string ShaderParser::directory = "../shader/sources/";
std::unordered_map<std::string, GLuint> ShaderParser::vertexShaders;
std::unordered_map<std::string, GLuint> ShaderParser::fragmentShaders;
GLuint ShaderParser::program;

bool ShaderParser::initialized = false;
bool ShaderParser::vertexReady = false;
bool ShaderParser::fragmentReady = false;

/* Functions */

/// Parses the given shader file into a string from the sources directory.
/// @param fileName Name of the file in the sources directory without '.shader'.
/// @return Parsed file as a string.
std::string ShaderParser::ReadShader(const std::string& name) {

    // Open File
    std::ifstream file(directory + name + ".shader");
    std::string parsed;

    // File to String
    if(file) {
        std::ostringstream ss;
        ss << file.rdbuf(); // reading data
        parsed = ss.str();
    } else return "";
    return parsed;
}

void ShaderParser::Unload() {
    for (auto& vertexShader : vertexShaders) {
        glDeleteShader(vertexShader.second);
    }
    for (auto& fragmentShader : fragmentShaders) {
        glDeleteShader(fragmentShader.second);
    }
    glDeleteProgram(program);
}

/// Loads the shader needed and compiles it.
/// @param name Name of the shader in question without '.shader' from sources directory.
/// @param type Type of shader loading.
void ShaderParser::LoadShader(const std::string& name, ShaderType type) {

    // Checked if already loaded shader.
    if((type == Vertex && !vertexShaders.count(name)) || (type == Fragment && !fragmentShaders.count(name))) {
        const std::string stringRead = ReadShader(name);

        // If string isn't empty load it.
        if(!stringRead.empty()) {
            GLuint id = (type == Vertex) ? glCreateShader(GL_VERTEX_SHADER) : glCreateShader(GL_FRAGMENT_SHADER);

            //Compile Shaders
            const GLchar* shaderSource = stringRead.c_str();
            glShaderSource(id, 1, &shaderSource, nullptr);
            glCompileShader(id);

            // Check for compilation errors
            GLint success;
            GLchar infoLog[512];
            glGetShaderiv(id, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(id, 512, nullptr, infoLog);
                switch(type) {
                    case Vertex:
                        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
                        break;
                    case Fragment:
                        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
                        break;
                }
                return;
            }
            (type == Vertex) ? vertexShaders[name] = id : fragmentShaders[name] = id;
        }
        else {
            switch(type) {
                case Vertex:
                    std::cerr << "ERROR::SHADER::VERTEX::READING_FAILED\n" << std::endl;
                    break;
                case Fragment:
                    std::cerr << "ERROR::SHADER::FRAGMENT::READING_FAILED\n" << std::endl;
                    break;
            }
        }
    } else return; // Already had shader.
}

/// Adds the shader name to "program" and links that program if ready.
/// Will load if not loaded.
/// @param name Name of the shader in question without '.shader' from sources directory.
/// @param type Type of the shader binding.
void ShaderParser::BindShader(const std::string& name, ShaderType type)
{
    LoadShader(name,type);
    if(!initialized)
    {
        program = glCreateProgram();
        initialized = true;
    }
    switch (type)
    {
        case Vertex:
            glAttachShader(program, vertexShaders[name]);
            vertexReady = true;
            break;
        case Fragment:
            glAttachShader(program, fragmentShaders[name]);
            fragmentReady = true;
            break;
    }
    if(fragmentReady && vertexReady)
    {
        glLinkProgram(program);

        // Check for linking errors
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            return;
        }

        glUseProgram(program);
    }
}

/// Gives us the status of our shader program.
/// \return If program is linked and ready.
bool ShaderParser::GetReady()
{
    return (fragmentReady && vertexReady);
}
