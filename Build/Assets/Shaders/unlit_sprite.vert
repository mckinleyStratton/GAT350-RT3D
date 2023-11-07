#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;

out layout(location = 0) vec2 otexcoord;

uniform struct Material
{
	vec3 albedo;
	
	vec2 offset;
	vec2 tiling;
} material;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	otexcoord = (vtexcoord * material.tiling) + material.offset;

	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vposition, 1.0);
}
