#version 430

// vertex position, texture coordinates, and vertex normal
in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

// in model/view space - fragment position, normal vector, and texture coordinates
out layout(location = 0) vec3 oposition;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec2 otexcoord;

// uniform variables for transformations - model matrix, view matrix, projection matrix
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//material properites
uniform struct Material 
{
    vec3 albedo;
    vec3 specular;
    vec3 emissive;
    float shininess;

    vec2 offset;
    vec2 tiling;
} material;

void main()
{
    mat4 modelView = view * model;

    // convert position and normal to world-view space - PHONG
    oposition = vec3(modelView * vec4(vposition, 1));
    onormal = normalize(mat3(modelView) * vnormal);
    otexcoord = (vtexcoord * material.tiling) + material.offset;

    // transform vertex position to the space used for rendering
    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(vposition, 1.0);
}
