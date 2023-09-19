//
// Created by Carter Thompson on 9/17/23.
//

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "SceneObject.h"
#include "glm/gtx/string_cast.hpp"
#include "../utility/stb_image.h"

SceneObject::SceneObject(const std::string& location, Transform aTransform) : Object("SceneObject") {
    transform = aTransform;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(location,
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        // Handle errors here
        std::cerr << "ERROR::ASSIMP::SCENE_READ_FAILED" << std::endl;
        delete(this);
    }

    ProcessNode(scene->mRootNode, scene, glm::identity<glm::mat4>());
    modelLocation = glGetUniformLocation(ShaderParser::program, "model");
}

void SceneObject::CreateMesh(const aiScene* scene, aiMesh* mesh, glm::mat4 meshTransform)
{
    auto* vertices = (float*) malloc(sizeof(float) * 3 * mesh->mNumVertices);

    for(int i = 0; i < mesh->mNumVertices; i++)
    {
        vertices[3 * i] = mesh->mVertices[i].x;
        vertices[3 * i + 1] = mesh->mVertices[i].y;
        vertices[3 * i + 2] = mesh->mVertices[i].z;
    }

    auto* normals = (float*) malloc(sizeof(float) * 3 * mesh->mNumVertices);

    for(int i = 0; i < mesh->mNumVertices; i++)
    {
        normals[3 * i] = mesh->mNormals[i].x;
        normals[3 * i + 1] = mesh->mNormals[i].y;
        normals[3 * i + 2] = mesh->mNormals[i].z;
    }

    auto* uv = (float*) malloc(sizeof(float) * 2 * mesh->mNumVertices);

    for(int i = 0; i < mesh->mNumVertices; i++)
    {
        aiVector3D vec = mesh->mTextureCoords[0][i];

        uv[2* i] = vec.x;
        uv[2 * i + 1] = vec.y;
    }

    auto *indices = (unsigned int*) malloc(sizeof(unsigned int) * 3 * mesh->mNumFaces);

    for(int i = 0; i < mesh->mNumFaces; i++)
    {
        indices[3 * i] = mesh->mFaces[i].mIndices[0];
        indices[3 * i + 1] = mesh->mFaces[i].mIndices[1];
        indices[3 * i + 2] = mesh->mFaces[i].mIndices[2];
    }

    meshes.emplace_back(std::make_unique<Mesh>(vertices, uv, normals, indices, mesh->mNumFaces * 3, mesh->mNumVertices, meshTransform));

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    aiString path;

    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {

        int width, height, nrChannels;

        // Load image
        unsigned char* raw_data = stbi_load(("../models/" + std::string(path.data)).c_str(), &width, &height, &nrChannels, 0);

        // Wrap it in a shared_ptr with custom deleter
        std::shared_ptr<unsigned char> data(raw_data, stbi_image_free);

        Image ourImage = {data, 0, width, height, nrChannels};
        glGenTextures(1, &ourImage.texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ourImage.texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ourImage.width, ourImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.get());
        glGenerateMipmap(GL_TEXTURE_2D);

        meshes.back()->AssignTexture(COLOR_TEXTURE, ourImage);
    }
}

void SceneObject::Render() {

    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transform.getModel()));
    for (auto& mesh: meshes) {
        mesh->DrawMesh();
    }
}

void SceneObject::ProcessNode(aiNode* node, const aiScene* scene, glm::mat4 parentTransform)
{
    // Convert the node's local transformation matrix to glm::mat4
    aiMatrix4x4 aiTransform = node->mTransformation;
    glm::mat4 localTransform = ConvertMatrix(aiTransform);

    // Calculate the global transformation for this node
    glm::mat4 globalTransform = parentTransform * localTransform;

    // Process all the meshes for this node
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        CreateMesh(scene, mesh, globalTransform);
    }

    // Recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene, globalTransform);
    }
}

glm::mat4 SceneObject::ConvertMatrix(const aiMatrix4x4 &from)
{
    glm::mat4 to;

    to[0][0] = from.a1; to[0][1] = from.b1; to[0][2] = from.c1; to[0][3] = from.d1;
    to[1][0] = from.a2; to[1][1] = from.b2; to[1][2] = from.c2; to[1][3] = from.d2;
    to[2][0] = from.a3; to[2][1] = from.b3; to[2][2] = from.c3; to[2][3] = from.d3;
    to[3][0] = from.a4; to[3][1] = from.b4; to[3][2] = from.c4; to[3][3] = from.d4;

    return to;
}
