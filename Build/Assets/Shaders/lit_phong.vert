#version 430
// VIEW SPACE (vert is called per vertex)

// vertex attributes
in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

// attributes passed to fragment shader
out layout(location = 0) vec3 oposition;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec2 otexcoord;

// pass in model matrix from world - used to transform vertex positions and normals
uniform mat4 model; 
uniform mat4 view;
uniform mat4 projection;

uniform struct Material
{
	uint params;
	vec3 albedo;
	vec3 specular;
	float shininess;
	vec3 emissive;

	vec2 offset;
	vec2 tiling;
} material;


void main()
{
	mat4 modelView = view * model;

	oposition = vec3(modelView * vec4(vposition, 1));

	onormal = normalize(mat3(modelView) * vnormal);

	otexcoord = (vtexcoord * material.tiling) + material.offset;

	mat4 mvp = projection * view * model; 
	gl_Position = mvp * vec4(vposition, 1.0);
}
