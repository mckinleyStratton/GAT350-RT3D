#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec2 otexcoord;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec4 ocolor;

// pass in model matrix from world 3
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform struct Material
{
	vec4 color;
	vec2 offset;
	vec2 tiling;
} material;


void main()
{
	otexcoord = (vtexcoord * material.tiling) + material.offset;
	onormal = vnormal;
	ocolor = material.color;

	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vposition, 1.0);
}
