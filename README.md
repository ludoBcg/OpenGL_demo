# OpenGL_demo
Base code for an OpenGL project

![opengl_demo](https://github.com/ludoBcg/OpenGL_demo/assets/84736834/e6ed8179-634e-460d-85d6-55b1f2ecabb9)

## 1. Dependencies

Create an *external* directory and copy into it the following libraries:

* [GLEW (The OpenGL Extension Wrangler Library)](http://glew.sourceforge.net/)

  Download the binaries for windows
  
* [GLM (OpenGL Mathematics)](https://glm.g-truc.net)

  Just download the libary (header only)

* [GLFW (Graphics Library Framework)](https://www.glfw.org/)

  Download latest version. The library is provided with a Cmake file, use it to generated a VisualStudio solution, then open it, and build

* [Dear ImGui (Immediate-mode Graphical User Interface)](https://github.com/ocornut/imgui)

  Just download the libary

## 2. Compilation

Use the CMakeList provided to generate a project. Make sure that the path to sources and libraries are correct. Compile the generated solution and run (change the working directory to your *Debug/Release* folder if necessary). An example mesh is provided in *models*.
