//
// Created by Carter Thompson on 9/13/23.
//

#ifndef SOFTGEN_RENDER_H
#define SOFTGEN_RENDER_H

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

void Render();
void Init();
void Unload();
void InputManager(GLFWwindow* window);
void MouseMovementCallback(GLFWwindow* window, double xpos, double ypos);

#endif //SOFTGEN_RENDER_H