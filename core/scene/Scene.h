//
// Created by Carter Thompson on 9/18/23.
//

#ifndef SOFTGEN_SCENE_H
#define SOFTGEN_SCENE_H

#include <vector>
#include "../object/types/SceneObject.h"
#include "../camera/SceneCamera.h"

class Scene {

private:
    Scene() = default;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    std::vector<std::shared_ptr<SceneObject>> objects;

public:

    static Scene& GetInstance() {
        static Scene instance; // Guaranteed to be destroyed, initialized on first use
        return instance;
    }

    SceneCamera camera{};

    void AddObject(const std::shared_ptr<SceneObject>& object) {
        objects.push_back(object);
    }

    void Render() {
        camera.AssignCamera();
        for (auto object : objects)
        {
            object->Render();
        }
    }

};


#endif //SOFTGEN_SCENE_H
