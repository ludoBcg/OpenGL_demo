/*********************************************************************************************************************
 *
 * main.cpp
 * 
 * OpenGL_demo
 * Ludovic Blache
 *
 *********************************************************************************************************************/


// Standard includes 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>
#include <algorithm>

// OpenGL includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "tools.h"
#include "drawablemesh.h"


// Window
GLFWwindow *m_window;           /*!<  GLFW window */
int m_winWidth = 1024;          /*!<  window width  */
int m_winHeight = 720;          /*!<  window height  */

Trackball m_trackball;          /*!<  model trackball */

// Scene
glm::vec3 m_centerCoords;       /*!<  coords of the center of the scene */
float m_radScene;               /*!< radius of the scene (i.e., diagonal of the BBox) */

// Light and cameras 
Camera m_camera;                /*!<  camera */
float m_zoomFactor;             /*!<  zoom factor */
glm::vec3 m_camPos;             /*!<  camera position */
glm::vec3 m_lightPos;           /*!<  light source position  */
glm::vec3 m_lightCol;           /*!<  light color */

// 3D objects
TriMesh* m_triMesh;             /*!<  triangle mesh */
DrawableMesh* m_drawMeshTeapot; /*!<  drawable object: mesh object */
DrawableMesh* m_drawMeshCube;   /*!<  drawable object: mesh object */

glm::mat4 m_modelMatrix;        /*!<  model matrix of the mesh */
    
GLuint m_defaultVAO;            /*!<  default VAO */

// shader programs
GLuint m_program;               /*!< handle of the program object (i.e. shaders) for shaded surface rendering */

// UI flags
bool m_showTeapot = true;

float m_specPow = 128.0f;

std::string shaderDir = "../../src/shaders/";   /*!< relative path to shaders folder  */
std::string modelDir = "../../models/";   /*!< relative path to meshes and textures files folder  */


// Functions definitions

void initialize();
void initScene(TriMesh *_triMesh);
void setupImgui(GLFWwindow *window);
void update();
void display();
void resizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void charCallback(GLFWwindow* window, unsigned int codepoint);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void cursorPosCallback(GLFWwindow* window, double x, double y);
void runGUI();
int main(int argc, char** argv);




    /*------------------------------------------------------------------------------------------------------------+
    |                                                      INIT                                                   |
    +-------------------------------------------------------------------------------------------------------------*/


void initialize()
{   
    // init scene parameters
    m_lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
    m_zoomFactor = 1.0f;

    // Setup background color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);    
        
    // init model matrix
    m_modelMatrix = glm::mat4(1.0f);

    // init triangle mesh (read OBJ file)
    m_triMesh = new TriMesh();
    m_triMesh->readFile(modelDir + "teapot.obj");
    // setup mesh rendering
    m_drawMeshTeapot = new DrawableMesh;
    m_drawMeshTeapot->createMeshVAO(*m_triMesh);

    // init cube mesh
    m_drawMeshCube = new DrawableMesh;
    m_drawMeshCube->createUnitCubeVAO();

    // init scene depending on object geom
    initScene(m_triMesh);

    // init shaders
    m_program = loadShaderProgram(shaderDir + "phong.vert", shaderDir + "phong.frag");

}


void initScene(TriMesh *_triMesh)
{
    m_triMesh->computeAABB();
    glm::vec3 bBoxMin = m_triMesh->getBBoxMin();
    glm::vec3 bBoxMax = m_triMesh->getBBoxMax();
    if(bBoxMin != bBoxMax)
    {
        // set the center of the scene to the center of the bBox
        m_centerCoords = glm::vec3( (bBoxMin.x + bBoxMax.x) * 0.5f, (bBoxMin.y + bBoxMax.y) * 0.5f, (bBoxMin.z + bBoxMax.z) * 0.5f );
    }
    m_radScene = glm::length(bBoxMax - bBoxMin) * 0.5f;

    // init camera and lightsource position (identical)
    m_camPos = glm::vec3(0.0f, m_radScene*0.6f, m_radScene*3.0f);
    m_lightPos = m_camPos;
    // init camera
    m_camera.init(0.01f, m_radScene*8.0f, 45.0f, 1.0f, m_winWidth, m_winHeight, m_camPos, glm::vec3(0.0f, 0.0f, 0.0f), 0);

    // init trackball
    m_trackball.init(m_winWidth, m_winHeight);
}


void setupImgui(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    // enable keyboard controls?
    ImGui::StyleColorsDark();
    // platform and renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}


    /*------------------------------------------------------------------------------------------------------------+
    |                                                     UPDATE                                                  |
    +-------------------------------------------------------------------------------------------------------------*/

void update()
{
    // update model matrix with trackball rotation
    m_modelMatrix = glm::translate( m_trackball.getRotationMatrix(), -m_centerCoords);
}



    /*------------------------------------------------------------------------------------------------------------+
    |                                                     DISPLAY                                                 |
    +-------------------------------------------------------------------------------------------------------------*/


void display()
{    
    // bind dedicated FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Clear window with background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Get MVP matrices
    glm::mat4 modelMat = m_modelMatrix;
    glm::mat4 viewMat = m_camera.getViewMatrix();
    glm::mat4 projMat = m_camera.getProjectionMatrix();

    // draw objects
    if(m_showTeapot)
        m_drawMeshTeapot->draw(m_program, modelMat, viewMat, projMat, m_lightPos, m_camPos, m_lightCol);
    else
        m_drawMeshCube->draw(m_program, modelMat, viewMat, projMat, m_lightPos, m_camPos, m_lightCol);

}


    /*------------------------------------------------------------------------------------------------------------+
    |                                                CALLBACK METHODS                                             |
    +-------------------------------------------------------------------------------------------------------------*/


void resizeCallback(GLFWwindow* window, int width, int height)
{
    m_winWidth = width;
    m_winHeight = height;
    glViewport(0, 0, m_winWidth, m_winHeight);

    // re-init trackball and camera
    m_camera.initProjectionMatrix(m_winWidth, m_winHeight, 1.0f, 0);
    m_camera.initProjectionMatrix(m_winWidth, m_winHeight, m_zoomFactor, 0);
    m_trackball.init(m_winWidth, m_winHeight);

    // keep drawing while resize
    update();
    display();

    // Swap between front and back buffer
    glfwSwapBuffers(m_window);
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (ImGui::GetIO().WantCaptureKeyboard) { return; }   // Skip other handling when ImGUI is used   

    // return to init positon when "R" pressed
    if (key == GLFW_KEY_R && action == GLFW_PRESS) 
    {
        // restart trackball
        m_trackball.reStart();
        // re-init zoom
        m_zoomFactor = 1.0f;

        // update camera
        m_camera.initProjectionMatrix(m_winWidth, m_winHeight, m_zoomFactor, 0);

    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        m_program = loadShaderProgram(shaderDir + "phong.vert", shaderDir + "phong.frag");
    }
}


void charCallback(GLFWwindow* window, unsigned int codepoint)
{}


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (ImGui::GetIO().WantCaptureMouse) { return; }  // Skip other handling when ImGUI is used   

    // get mouse cursor position
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    // activate/de-activate trackball with mouse button
    if (action == GLFW_PRESS) 
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            m_trackball.startTracking( glm::vec2(x, y) );
    }
    else 
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            m_trackball.stopTracking();
    }
}


void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (ImGui::GetIO().WantCaptureMouse) { return; }  // Skip other handling when ImGUI is used  

    // update zoom factor
    double newZoom = m_zoomFactor - yoffset / 10.0f;
    if(newZoom > 0.0f && newZoom < 2.0f)
    {
        m_zoomFactor -= (float)yoffset/10.0f;
    }
    // update camera
    m_camera.initProjectionMatrix(m_winWidth, m_winHeight, m_zoomFactor, 0);

}


void cursorPosCallback(GLFWwindow* window, double x, double y)
{
    // rotate trackball according to mouse cursor movement
    if ( m_trackball.isTracking() ) 
        m_trackball.move( glm::vec2(x, y) );
}




    /*------------------------------------------------------------------------------------------------------------+
    |                                                      MAIN                                                   |
    +-------------------------------------------------------------------------------------------------------------*/

void runGUI()
{

    if(ImGui::Begin("Settings"))
    {

        // ImGui frame rate measurement
        float frameRate = ImGui::GetIO().Framerate;
        ImGui::Text("FrameRate: %.3f ms/frame (%.1f FPS)", 1000.0f / frameRate, frameRate);

        ImGui::Separator();

        if (ImGui::Button("Show teapot"))
        {
            m_showTeapot = true;
        }
        if (ImGui::Button("Show cube"))
        {
            m_showTeapot = false;
        }
        if (ImGui::SliderFloat("specular pow", &m_specPow, 0.5f, 200.0f, "%.2f"))
        {
            m_drawMeshTeapot->setSpeculatPower(m_specPow);
        }
    } // end "Settings"

    
    ImGui::End();

    // render
    ImGui::Render();
}

int main(int argc, char** argv)
{

    /* Initialize GLFW and create a window */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // <-- activate this line on MacOS
    m_window = glfwCreateWindow(m_winWidth, m_winHeight, "OpenGL demo", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, resizeCallback);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetCharCallback(m_window, charCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetScrollCallback(m_window, scrollCallback);
    glfwSetCursorPosCallback(m_window, cursorPosCallback);

    // init ImGUI
    setupImgui(m_window);


    // init GL extension wrangler
    glewExperimental = true;
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }
    std::cout << std::endl
              << "UI commands:" << std::endl
              << " - Mouse left button : trackball" << std::endl
              << " - Mouse scroll: camera zoom" << std::endl
              << " - R: re-init trackball and cameras" << std::endl << std::endl
              << "OpenGL version: " << glGetString(GL_VERSION) << std::endl
              << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

    glGenVertexArrays(1, &m_defaultVAO);
    glBindVertexArray(m_defaultVAO);

    // call init function
    initialize();

    // main rendering loop
    while (!glfwWindowShouldClose(m_window)) 
    {
        // process events
        glfwPollEvents();
        // start frame for ImGUI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // build GUI
        runGUI();

        // idle updates
        update();

        // render scene
        display();

        // render GUI
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // Swap between front and back buffer
        glfwSwapBuffers(m_window);
    }


    // Cleanup imGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Close window
    glfwDestroyWindow(m_window);
    glfwTerminate();


    return 0;
}
