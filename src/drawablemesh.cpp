/*********************************************************************************************************************
 *
 * drawablemesh.cpp
 * 
 * OpenGL_demo
 * Ludovic Blache
 *
 *********************************************************************************************************************/

#include "drawablemesh.h"


DrawableMesh::DrawableMesh()
{

    m_defaultVAO = 0;

    m_ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
    m_diffuseColor = glm::vec3(0.8f, 0.6f, 0.5f);
    m_specularColor = glm::vec3(0.95f, 0.95f, 0.95f);

    m_specPow = 128.0f;

}


DrawableMesh::~DrawableMesh()
{
    glDeleteBuffers(1, &(m_vertexVBO));
    glDeleteBuffers(1, &(m_normalVBO));

    glDeleteBuffers(1, &(m_indexVBO));
    glDeleteVertexArrays(1, &(m_meshVAO));
}


void DrawableMesh::createMeshVAO(TriMesh& _triMesh)
{
    // read vertices from mesh object and fill in VAO
    
    // mandatory data
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices;      // !! uint32_t !!


    _triMesh.getVertices(vertices);
    _triMesh.getNormals(normals);
    _triMesh.getIndices(indices);


    // Generates and populates a VBO for vertex coords
    glGenBuffers(1, &(m_vertexVBO));
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    size_t verticesNBytes = vertices.size() * sizeof(vertices[0]);
    glBufferData(GL_ARRAY_BUFFER, verticesNBytes, vertices.data(), GL_STATIC_DRAW);

    // Generates and populates a VBO for vertex normals
    glGenBuffers(1, &(m_normalVBO));
    glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
    size_t normalsNBytes = normals.size() * sizeof(normals[0]);
    glBufferData(GL_ARRAY_BUFFER, normalsNBytes, normals.data(), GL_STATIC_DRAW);

    // Generates and populates a VBO for the element indices
    glGenBuffers(1, &(m_indexVBO));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
    auto indicesNBytes = indices.size() * sizeof(indices[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesNBytes, indices.data(), GL_STATIC_DRAW);



    // Creates a vertex array object (VAO) for drawing the mesh
    glGenVertexArrays(1, &(m_meshVAO));
    glBindVertexArray(m_meshVAO);

    // add vertex pos to VAO
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    glEnableVertexAttribArray(POSITION);
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // add vertex normals to VAO
    glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
    glEnableVertexAttribArray(NORMAL);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
    glBindVertexArray(m_defaultVAO); // unbinds the VAO

    // Additional information required by draw calls
    m_numVertices = (int)vertices.size();
    m_numIndices = (int)indices.size();

    // Clear temporary vectors
    vertices.clear();
    normals.clear();
    indices.clear();
}


void DrawableMesh::createUnitCubeVAO()
{

    std::vector<glm::vec3> vertices = { glm::vec3(-0.5f,  0.5f,  -0.5f), glm::vec3(-0.5f,  -0.5f,  -0.5f), glm::vec3(0.5f,  -0.5f,  -0.5f), glm::vec3(0.5f,  0.5f,  -0.5f),
                                        glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-0.5f,  -0.5f,  0.5f), glm::vec3(0.5f,  -0.5f,  0.5f), glm::vec3(0.5f,  0.5f,  0.5f) };

    std::vector<glm::vec3> normals = { glm::vec3(-0.33f,  0.33f, -0.33f), glm::vec3(-0.33f, -0.33f, -0.33f), glm::vec3(0.33f, -0.33f, -0.33f), glm::vec3(0.33f,  0.33f, -0.33f),
                                       glm::vec3(-0.33f,  0.33f,  0.33f), glm::vec3(-0.33f, -0.33f,  0.33f), glm::vec3(0.33f, -0.33f,  0.33f), glm::vec3(0.33f,  0.33f,  0.33f) };


    std::vector<uint32_t> indices{ 2, 1, 0, 0, 3, 2,   // back face
                                   5, 6, 7, 7, 4, 5,   // front face
                                   1, 5, 4, 4, 0, 1,   // left face
                                   6, 2, 3, 3, 7, 6,   // right face
                                   3, 0, 4, 4, 7, 3,   // top face
                                   6, 5, 1, 1, 2, 6 }; // bottom face


    // Generates and populates a VBO for vertex coords
    glGenBuffers(1, &(m_vertexVBO));
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    size_t verticesNBytes = vertices.size() * sizeof(vertices[0]);
    glBufferData(GL_ARRAY_BUFFER, verticesNBytes, vertices.data(), GL_STATIC_DRAW);

    // Generates and populates a VBO for vertex normals
    glGenBuffers(1, &(m_normalVBO));
    glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
    size_t normalsNBytes = normals.size() * sizeof(normals[0]);
    glBufferData(GL_ARRAY_BUFFER, normalsNBytes, normals.data(), GL_STATIC_DRAW);

    // Generates and populates a VBO for the element indices
    glGenBuffers(1, &(m_indexVBO));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
    auto indicesNBytes = indices.size() * sizeof(indices[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesNBytes, indices.data(), GL_STATIC_DRAW);


    // Creates a vertex array object (VAO) for drawing the mesh
    glGenVertexArrays(1, &(m_meshVAO));
    glBindVertexArray(m_meshVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    glEnableVertexAttribArray(POSITION);
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
    glEnableVertexAttribArray(NORMAL);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
    glBindVertexArray(m_defaultVAO); // unbinds the VAO

    // Additional information required by draw calls
    m_numVertices = (int)vertices.size();
    m_numIndices = (int)indices.size();

    // Clear temporary vectors
    vertices.clear();
    normals.clear();
    indices.clear();
}



void DrawableMesh::draw(GLuint _program, glm::mat4& _modelMat, glm::mat4& _viewMat, glm::mat4& _projMat, glm::vec3& _lightPos,  glm::vec3& _camPos,  glm::vec3& _lightCol)
{
    // Activate program
    glUseProgram(_program);


    // Pass uniforms
    glUniformMatrix4fv(glGetUniformLocation(_program, "u_matM"), 1, GL_FALSE, &_modelMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(_program, "u_matV"), 1, GL_FALSE, &_viewMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(_program, "u_matP"), 1, GL_FALSE, &_projMat[0][0]);
    glUniform3fv(glGetUniformLocation(_program, "u_lightPos"), 1, &_lightPos[0]);
    glUniform3fv(glGetUniformLocation(_program, "u_camPos"), 1, &_camPos[0]);

    glUniform3fv(glGetUniformLocation(_program, "u_lightColor"), 1, &_lightCol[0]);

    glUniform3fv(glGetUniformLocation(_program, "u_ambientColor"), 1, &m_ambientColor[0]);
    glUniform3fv(glGetUniformLocation(_program, "u_diffuseColor"), 1, &m_diffuseColor[0]);
    glUniform3fv(glGetUniformLocation(_program, "u_specularColor"), 1, &m_specularColor[0]);
    glUniform1f(glGetUniformLocation(_program, "u_specularPower"), m_specPow);

    // ...

    // Draw!
    glBindVertexArray(m_meshVAO);                       // bind the VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);  // do not forget to bind the index buffer AFTER !

    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(m_defaultVAO);

    glUseProgram(0);
}


