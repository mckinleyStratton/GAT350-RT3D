#version 430

// input for modelview space - 3D position, 3D normal vecor, 2D texture coordinates
in vec3 fposition;
in vec3 fnormal;
in vec2 ftexcoord;


// output color
out vec4 ocolor;

// texture sampler
layout(binding = 0) uniform sampler2D tex;

// material properties
uniform struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
    vec2 offset;
    vec2 tiling;
} material;

// light properties
uniform struct Light {
    vec3 position;
    vec3 color;
} light;

// ambient light color
uniform vec3 ambientLight;

// calculating ambient, diffuse and specular lighting with PHONG method
vec3 ads(vec3 position, vec3 normal) {

    // AMBIENT
    vec3 ambient = ambientLight;

    // DIFFUSE
    vec3 lightDir = normalize(light.position - position);
    float intensity = max(dot(lightDir, normal), 0);
    vec3 diffuse = material.diffuse * (light.color * intensity);

    // SPECULAR
    vec3 specular = vec3(0);
    if (intensity > 0)
    {
        vec3 reflection = reflect(-lightDir, normal);
        vec3 viewDir = normalize(-position);
        intensity = max(dot(reflection, viewDir), 0);
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
