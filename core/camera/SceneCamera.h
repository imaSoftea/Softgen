//
// Created by Carter Thompson on 9/18/23.
//

#ifndef SOFTGEN_SCENE_CAMERA_H
#define SOFTGEN_SCENE_CAMERA_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <OpenGL/gl3.h>
#include "../object/utility/Transform.h"
#include "../../shader/ShaderParser.h"

class SceneCamera {

private:
    int viewLocation;
    int projectionLocation;

    // Mouse Controls
    bool firstMouse {false};
    float lastX{};
    float lastY{};

    float yaw;
    float pitch;

public:
    glm::vec3 position{};
    glm::vec3 cameraDirectionVector{};
    glm::vec3 upVector;

    glm::mat4 projection{};
    glm::mat4 view{};

    SceneCamera()
    {
        viewLocation = glGetUniformLocation(ShaderParser::program, "view");
        projectionLocation = glGetUniformLocation(ShaderParser::program, "projection");

        position = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);

        cameraDirectionVector = glm::normalize(target- position);

        upVector = glm::vec3{0.0f,1.0f,0.0};
        glm::vec3 rightVector = glm::normalize(glm::cross(upVector, cameraDirectionVector));

        projection = glm::perspective(glm::radians(45.0f), (float)640/(float)480, 0.01f, 100.0f);

        yaw = -90;
        pitch = 0;

    }

    void AssignCamera()
    {
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    }

    void ProcessInput(GLFWwindow* window)
    {
        const float cameraSpeed = 0.05f; // adjust accordingly

        // Forward & Backwards
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            position += cameraSpeed * cameraDirectionVector;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            position -= cameraSpeed * cameraDirectionVector;

        // Upwards & Downwards
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            position += cameraSpeed * upVector;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            position -= cameraSpeed * upVector;

        // Left & Right
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            position -= glm::normalize(glm::cross(cameraDirectionVector, upVector)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            position += glm::normalize(glm::cross(cameraDirectionVector, upVector)) * cameraSpeed;

        //Look At
        view = glm::lookAt(position, position + cameraDirectionVector, upVector);
    }

    void mouse_callback(GLFWwindow* window, double xPosition, double yPosition)
    {
        if (firstMouse)
        {
            lastX = xPosition;
            lastY = yPosition;
            firstMouse = false;
        }

        float x_offset = xPosition - lastX;
        float y_offset = lastY - yPosition;
        lastX = xPosition;
        lastY = yPosition;

        float sensitivity = 0.4f;
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
            sensitivity = 0.0f;

        x_offset *= sensitivity;
        y_offset *= sensitivity;

        yaw += x_offset;
        pitch += y_offset;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraDirectionVector = glm::normalize(direction);
    }
};


#endif //SOFTGEN_SCENE_CAMERA_H
