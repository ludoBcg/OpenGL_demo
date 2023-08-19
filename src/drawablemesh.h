/*********************************************************************************************************************
 *
 * drawablemesh.h
 *
 * Buffer manager for mesh rendering
 * 
 * OpenGL_demo
 * Ludovic Blache
 *
 *********************************************************************************************************************/

#ifndef DRAWABLEMESH_H
#define DRAWABLEMESH_H

#define QT_NO_OPENGL_ES_2
#include <GL/glew.h>


#include "trimesh.h"

// The attribute locations we will use in the vertex shader
enum AttributeLocation 
{
    POSITION = 0,
    NORMAL = 1,
};



/*!
* \class DrawableMesh
* \brief Drawable mesh
* Render a TriMesh using a Blinn-Phong shading model and texture mapping
*/
class DrawableMesh
{
    public:

        /*------------------------------------------------------------------------------------------------------------+
        |                                        CONSTRUCTORS / DESTRUCTORS                                           |
        +------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn DrawableMesh
        * \brief Default constructor of DrawableMesh
        */
        DrawableMesh();


        /*!
        * \fn ~DrawableMesh
        * \brief Destructor of DrawableMesh
        */
        ~DrawableMesh();


        /*------------------------------------------------------------------------------------------------------------+
        |                                              GETTERS/SETTERS                                                |
        +-------------------------------------------------------------------------------------------------------------*/


        /*! \fn setSpeculatPower */
        inline void setSpeculatPower(float _specPow) { m_specPow = _specPow; }

        /*! \fn setAmbientColor */
        inline void setAmbientColor(int _r, int _g, int _b) { m_ambientColor = glm::vec3( (float)_r/255.0f, (float)_g/255.0f, (float)_b/255.0f ); }
        /*! \fn setAmbientColor */
        inline void setDiffuseColor(int _r, int _g, int _b) { m_diffuseColor = glm::vec3( (float)_r/255.0f, (float)_g/255.0f, (float)_b/255.0f ); }
        /*! \fn setAmbientColor */
        inline void setSpecularColor(int _r, int _g, int _b) { m_specularColor = glm::vec3( (float)_r/255.0f, (float)_g/255.0f, (float)_b/255.0f ); }



        /*------------------------------------------------------------------------------------------------------------+
        |                                               OTHER METHODS                                                 |
        +-------------------------------------------------------------------------------------------------------------*/

        /*!
        * \fn createMeshVAO
        * \brief Create mesh VAO and VBOs.
        * \param _triMesh : Mesh to update mesh VAO and VBOs from
        */
        void createMeshVAO(TriMesh& _triMesh);

        /*!
        * \fn createUnitCubeVAO
        * \brief Create cube VAO and VBOs (for skybox).
        * \param _centerCoords : center of the scene (for floor quad only) 
        * \param _radScene : Radius of the scene (for floor quad only) 
        */
        void createUnitCubeVAO();

        /*!
        * \fn draw
        * \brief Draw the content of the mesh VAO
        * \param _program : shader program
        * \param _modelMat : model matrix
        * \param _viewMat :camera view matrix
        * \param _projMat :camera projection matrix
        * \param _lightPos : 3D coords of light position
        * \param _lightCol : RGB color of the light
        */
        void draw(GLuint _program, glm::mat4& _modelMat, glm::mat4& _viewMat, glm::mat4& _projMat, glm::vec3& _lightPos, glm::vec3& _camPos, glm::vec3& _lightCol);

        


    protected:

        /*------------------------------------------------------------------------------------------------------------+
        |                                                ATTRIBUTES                                                   |
        +-------------------------------------------------------------------------------------------------------------*/

        GLuint m_meshVAO;           /*!< mesh VAO */
        GLuint m_defaultVAO;        /*!< default VAO */

        GLuint m_vertexVBO;         /*!< name of vertex 3D coords VBO */
        GLuint m_normalVBO;         /*!< name of normal vector VBO */

        GLuint m_indexVBO;          /*!< name of index VBO */

        int m_numVertices;          /*!< number of vertices in the VBOs */
        int m_numIndices;           /*!< number of indices in the index VBO */

        float m_specPow;            /*!< specular power */

        glm::vec3 m_ambientColor;   /*!< ambient color */
        glm::vec3 m_diffuseColor;   /*!< diffuse color */
        glm::vec3 m_specularColor;  /*!< specular color */

        bool m_vertexProvided;      /*!< flag to indicate if vertex coords are available or not */
        bool m_normalProvided;      /*!< flag to indicate if normals are available or not */
        bool m_indexProvided;       /*!< flag to indicate if indices are available or not */

};
#endif // DRAWABLEMESH_H
