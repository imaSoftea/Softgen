//
// Created by Carter Thompson on 9/13/23.
//
#include <GLFW/glfw3.h>
#include <assimp/DefaultLogger.hpp>

#include "Render.h"
#include "object/types/SceneObject.h"
#include "scene/Scene.h"

void Render() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Generate and bind VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Depth Testing
    glEnable(GL_DEPTH_TEST);

    // Draw Scene
    Scene::GetInstance().Render();

    // Unbind VAO (optional)
    glBindVertexArray(0);
}

void InputManager(GLFWwindow* window) {
    Scene::GetInstance().camera.ProcessInput(window);
    glfwSetCursorPosCallback(window,  MouseMovementCallback);
};

void Init() {
    ShaderParser::BindShader("vertex", Vertex);
    ShaderParser::BindShader("fragment", Fragment);
    Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);

    //Initialize Objects In Scene
    Scene::GetInstance().AddObject(std::make_shared<SceneObject>("../models/OPENGL.dae", Transform({0,0,-10})));

}

void Unload() {
    ShaderParser::Unload();
}

void MouseMovementCallback(GLFWwindow* window, double xpos, double ypos)
{
    Scene::GetInstance().camera.mouse_callback(window, xpos, ypos);
}