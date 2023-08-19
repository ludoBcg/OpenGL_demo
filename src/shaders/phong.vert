// Vertex shader
#version 330
#extension GL_ARB_shader_image_load_store : require

// VERTEX ATTRIBUTES
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec3 a_normal;



// UNIFORMS
uniform mat4 u_matM;
uniform mat4 u_matV;
uniform mat4 u_matP;
uniform vec3 u_lightPos;

// OUTPUT
out vec3 vecN;
out vec3 vecV;
out vec3 vecL;



void main()
{
    // ModelView matrix
    mat4 matMV = u_matV * u_matM;
    // ModelViewProjection matrix
    mat4 matMVP = u_matP * matMV;

    vec3 v_pos = vec3(matMV * a_position);

    // Calculate the view-space normal
    vecN = normalize(mat3(matMV) * a_normal);

    // Calculate the view-space light direction
    vec3 l_vecLight = mat3(u_matV) * u_lightPos;
    vecL = normalize(l_vecLight - v_pos);

    vecV = -normalize(v_pos);
    
    gl_Position = matMVP * a_position;
}

