//
// Created by Carter Thompson on 9/17/23.
//

#ifndef SOFTGEN_TRANSFORM_H
#define SOFTGEN_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
private:
    glm::vec3 position{0};
public:

    Transform() = default;
    Transform(glm::vec3 pos)
    {
        position = pos;
    }

    glm::mat4 getModel()
    {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        return model;
    }
};

#endif //SOFTGEN_TRANSFORM_H
