//
// Created by Carter Thompson on 9/17/23.
//

#ifndef SOFTGEN_SCENEOBJECT_H
#define SOFTGEN_SCENEOBJECT_H

#include <unordered_map>
#include <vector>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "../Object.h"
#include "../mesh/Mesh.h"
#include "../utility/Transform.h"



class SceneObject : Object {

private:
    std::vector<std::unique_ptr<Mesh>> meshes;
    int modelLocation;
    Transform transform;

    void CreateMesh(const aiScene* scene, aiMesh* mesh, glm::mat4 meshTransform);
    void ProcessNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform);
    glm::mat4 ConvertMatrix(const aiMatrix4x4 &from);

public:
    void Render() override;

    SceneObject(const std::string& location, Transform aTransform);

};


#endif //SOFTGEN_SCENEOBJECT_H
