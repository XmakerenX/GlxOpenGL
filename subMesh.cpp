#include "subMesh.h"

//-----------------------------------------------------------------------------
// Name : SubMesh (constructor)
//-----------------------------------------------------------------------------
SubMesh::SubMesh(std::vector<Vertex> vertices, std::vector<GLushort> indices)
{
    this->vertices = vertices;
    this->indices = indices;

    // Now that we have all the required data, set the vertex buffers and its attribute pointers.
    this->setupMesh();
}

//-----------------------------------------------------------------------------
// Name : Draw
//-----------------------------------------------------------------------------
void SubMesh::Draw()
{
    // Draw mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLE_STRIP, this->indices.size(), GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);

}

//-----------------------------------------------------------------------------
// Name : setupMesh
//-----------------------------------------------------------------------------
void SubMesh::setupMesh()
{
    // Create buffers/arrays
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLushort), this->indices.data(), GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
