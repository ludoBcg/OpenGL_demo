// Fragment header
#version 330

// UNIFORMS
uniform vec3 u_lightColor;

uniform vec3 u_ambientColor;
uniform vec3 u_diffuseColor;
uniform vec3 u_specularColor;
uniform float u_specularPower;
    
// INPUT
in vec3 vecN;
in vec3 vecV;
in vec3 vecL;


// OUTPUT
out vec4 frag_color;


float diffuse(in vec3 _N, in vec3 _L)
{
    // Calculate the diffuse (Lambertian) reflection term
    return max(0.0, dot(_N, _L));
}

float specular_normalized(in vec3 _N, in vec3 _H, in float _specularPower)
{
    float normalization = (8.0 + _specularPower) / 8.0;
    float specular = normalization * pow(max(0.0, dot(_N, _H)), _specularPower); // add max to remove black artifacts
    specular = min(1.0f, specular); // make sure max specular value is 1
    return specular;
}

// MAIN
void main()
{

    // final color
    vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    vec3 diff_col = u_diffuseColor;
        
    // -- Render Blinn-Phong shading --
    vec3 vecH = normalize(vecL + vecV);
    
    //DIFFUSE
    float diffuse = diffuse(vecN, vecL);
    color.rgb += diff_col * u_lightColor * diffuse;

    //SPECULAR
    float specular = specular_normalized(vecN, vecH, u_specularPower);
    color.rgb += u_specularColor * u_lightColor * specular;

    //AMBIENT
    color.rgb += u_ambientColor;


    frag_color = color;

}

