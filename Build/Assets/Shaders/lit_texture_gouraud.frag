#version 430

// input for modelview space - 3D position, 3D normal vecor, 2D texture coordinates, and color with RGBA components

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;
in layout(location = 3) vec4 fcolor;

// output color
out layout(location = 0) vec4 ocolor;

// texture sampler
layout(binding = 0) uniform sampler2D tex;

void main()
{
    // texure color
	vec4 texcolor = texture(tex,ftexcoord);

	// calculate final color GOURAUD method
	ocolor = texcolor * fcolor;
}

