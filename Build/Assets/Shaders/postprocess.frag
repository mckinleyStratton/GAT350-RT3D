#version 430


# define INVERT_MASK		(1 << 0)
# define GRAYSCALE_MASK		(1 << 1)
# define COLORTINT_MASK		(1 << 2)
# define GRAIN_MASK			(1 << 3)
# define SCANLINE_MASK		(1 << 4)
# define CUSTOM_MASK		(1 << 5)


in	layout(location = 0) vec2 ftexcoord;
out layout(location = 0) vec4 ocolor;

uniform float blend = 1;
uniform uint params = 0;

uniform float rTint = 1;
uniform float bTint = 1;
uniform float gTint = 1;




layout(binding = 0) uniform sampler2D screenTexture;

vec4 invert(in vec4 color)
{
	return vec4(vec3(1) - color.rgb, color.a);
}

vec4 grayscale(in vec4 color)
{
	return vec4(vec3(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b), color.a);
}

// color tints
vec4 colortint(in vec4 color)
{
    return vec4(vec3(rTint, gTint, bTint) * color.rgb, color.a);
}



// grain one 
// scan line - every other pixel line is black

void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;

	if (bool(params & INVERT_MASK))		postprocess = invert(postprocess);
	if (bool(params & GRAYSCALE_MASK))	postprocess = grayscale(postprocess);
	if (bool(params & COLORTINT_MASK))	postprocess = colortint(postprocess);

	ocolor = mix(basecolor, postprocess, blend);
}
