#version 430

// vertex position, texture coordinates, and vertex normal
layout(location = 0) in vec3 vposition;
layout(location = 1) in vec2 vtexcoord;
layout(location = 2) in vec3 vnormal;

// in model/view space - fragment position, normal vector, and texture coordinates
out vec3 fposition;
out vec3 fnormal;
out vec2 ftexcoord;

// uniform variables for transformations - model matrix, view matrix, projection matrix
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//material properites
uniform struct Material
{
    vec3 diffuse;
    vec3 specular;
    float shininess;

    vec2 offset;
    vec2 tiling;
} material;


void main()
{
    vec4 modelViewPosition = view * model * vec4(vposition, 1.0);

    // convert position and normal to world-view space - PHONG
    fposition = modelViewPosition.xyz;
    fnormal = normalize(mat3(transpose(inverse(model))) * vnormal);
    ftexcoord = vtexcoord * material.tiling + material.offset;

    // transform vertex position to the space used for rendering
    gl_Position = projection * modelViewPosition;
}
