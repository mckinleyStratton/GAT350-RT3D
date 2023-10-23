#version 430

#define POINT       0
#define DIRECTIONAL 1
#define SPOT        2


// input for modelview space - 3D position, 3D normal vecor, 2D texture coordinates
in vec3 fposition;
in vec3 fnormal;
in vec2 ftexcoord;


// output color
out vec4 ocolor;

// texture sampler
layout(binding = 0) uniform sampler2D tex;

// material properties
uniform struct Material 
{
    vec3 diffuse;
    vec3 specular;
    float shininess;

    vec2 offset;
    vec2 tiling;
} material;

// light properties
uniform struct Light 
{
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float cutoff;
} light;

// ambient light color
uniform vec3 ambientLight;

// calculating ambient, diffuse and specular lighting with PHONG method
vec3 ads(vec3 position, vec3 normal) {

    // AMBIENT
    vec3 ambient = ambientLight;

    // DIFFUSE
    vec3 lightDir = (light.type == DIRECTIONAL) ? normalize(-light.direction) : normalize(light.position - position);
    
    float spotIntensity = 1;
    if (light.type == SPOT)
    {
        float angle = acos(dot(light.direction, -lightDir));
        if (angle > light.cutoff) spotIntensity = 0;
    }
    
    
    float intensity = max(dot(lightDir, normal), 0);
    vec3 diffuse = material.diffuse * (light.color * intensity);
    //vec3 diffuse = material.diffuse * (light.color * intensity * spotIntensity);




    // SPECULAR
    vec3 specular = vec3(0);
    if (intensity > 0)
    {
        vec3 reflection = reflect(-lightDir, normal);
        vec3 viewDir = normalize(-position);
        float intensity = max(dot(lightDir, normal), 0) * spotIntensity;
        //intensity = max(dot(reflection, viewDir), 0);
        intensity = pow(intensity, material.shininess);
        specular = material.specular * intensity;
    }

    // calculate lighting contributions
    return ambient + diffuse + specular;
}

void main() {

    // texure color
    vec4 texcolor = texture(tex, ftexcoord);

    // calculate final color by combining texture color and PHONG lighting
    ocolor = texcolor * vec4(ads(fposition, fnormal), 1);

}
