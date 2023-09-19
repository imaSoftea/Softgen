//
// Created by Carter Thompson on 9/13/23.
//

#ifndef SOFTGEN_SHADERPARSER_H
#define SOFTGEN_SHADERPARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

#include <GLFW/glfw3.h>

enum ShaderType {
    Vertex,
    Fragment
};

class ShaderParser {

private:
    static const std::string directory;
    static bool initialized;
    static bool vertexReady;
    static bool fragmentReady;

    static std::unordered_map<std::string , GLuint> vertexShaders; // Map of Used Shaders
    static std::unordered_map<std::string , GLuint> fragmentShaders; // Map of Used Shaders

    static std::string ReadShader(const std::string& name); // Reads Shader (Empty string if failed)

public:
    static GLuint program;

    static void Unload();
    static void LoadShader(const std::string& name, ShaderType type); // Loads Shader (True if loaded - False if failed)
    static void BindShader(const std::string& name, ShaderType type); // Binds Shader (True if loaded - False if failed)
    static bool GetReady();
};

#endif //SOFTGEN_SHADERPARSER_H
