//
// Created by Carter Thompson on 9/17/23.
//

#ifndef SOFTGEN_MESH_H
#define SOFTGEN_MESH_H

#include <OpenGL/gl3.h>
#include <glm/fwd.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include "../../../shader/ShaderParser.h"
#include "glm/gtc/type_ptr.hpp"

enum TextureType {
    COLOR_TEXTURE,
    UNDEFINED_TEXTURE
};

struct {
    std::shared_ptr<unsigned char> data;
    unsigned int texture;
    int width, height, nrChannels;
} typedef Image;

class Mesh {

private:
    float* vertices;
    float* normals;
    float* uvCoords;
    unsigned int* indices;
    glm::mat4 transformationMatrix;
    int meshTransformLocation;

    int verticesCount;
    int indicesCount;
    std::unordered_map<TextureType, Image> textureData;

    //Vertices = 0; Normal = 1; UV = 2;
    unsigned int VBO[3]{};

    unsigned int EBO{};

    void BufferDataCreate() {

        glGenBuffers(3, &VBO[0]);
        glGenBuffers( 1, &EBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(verticesCount * sizeof(float)) * 3, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(verticesCount * sizeof(float) * 3), normals, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(verticesCount * sizeof(float) * 2), uvCoords, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indicesCount * sizeof(unsigned int)), indices, GL_STATIC_DRAW);
    }

    void BindBuffers()
    {
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glVertexAttribPointer(0, 3,  GL_FLOAT, false, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, nullptr);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }

public:

    Mesh(float* vert, float* norm, float* uv, unsigned int* ind, int indCount, int vertCount, glm::mat4 transform) {
        vertices = vert;
        normals = norm;
        uvCoords = uv;
        indices = ind;
        indicesCount = indCount;
        verticesCount = vertCount;

        transformationMatrix = transform;
        meshTransformLocation = glGetUniformLocation(ShaderParser::program, "mesh");

        BufferDataCreate();
    }

    void AssignTexture(TextureType type, Image texture)
    {
        textureData[type] = texture;
    }

    ~Mesh() {
        free(vertices);
        free(normals);
        free(uvCoords);
        free(indices);
    }

    void DrawMesh()
    {
        //Buffer Bind
        BindBuffers();

        //Assign Textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureData[COLOR_TEXTURE].texture);

        //Mesh Transformation Bind
        glUniformMatrix4fv(meshTransformLocation, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

        //Draw
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL Error: " << error << std::endl;
        }

    }

};


#endif //SOFTGEN_MESH_H
