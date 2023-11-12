#version 430


# define INVERT_MASK		(1 << 0)
# define GRAYSCALE_MASK		(1 << 1)
# define GRAIN_MASK			(1 << 2)
# define SCANLINE_MASK		(1 << 3)
# define CUSTOM_MASK		(1 << 4)
# define COLORTINT_MASK		(1 << 5)


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

// colorTint mask
vec4 colortint(in vec4 color)
{
    return vec4(vec3(rTint, gTint, bTint) * color.rgb, color.a);
}


// grain mask 
// can not include our rand.h and those funtions in a .frag
float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}
vec4 grain(in vec4 color)
{
	float grainStrength = 0.7;
    float randValue = rand(ftexcoord);
    vec3 grainColor = mix(color.rgb, color.rgb - vec3(randValue * grainStrength), grainStrength);

    return vec4(grainColor, color.a);
}

// scan line mask
vec4 scanline(in vec4 color)
{
    float scanlineIntensity = 1;
    float scanlineSpacing = 1.0;
    float scanlineThickness = 0.5;

    float scanlinePosition = mod(ftexcoord.y * 100.0, scanlineSpacing);

    float scanlineMask = smoothstep(0.0, scanlineThickness, abs(mod(scanlinePosition, scanlineSpacing) - scanlineSpacing / 2.0));

    color.rgb -= color.rgb * scanlineIntensity * scanlineMask;

    return color;
}




void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;

	if (bool(params & INVERT_MASK))		postprocess = invert(postprocess);
	if (bool(params & GRAYSCALE_MASK))	postprocess = grayscale(postprocess);
	if (bool(params & GRAIN_MASK))		postprocess = grain(postprocess);
	if (bool(params & SCANLINE_MASK))	postprocess = scanline(postprocess);
	//if (bool(params & PIXEL_MASK))	postprocess = pixel(postprocess);
	if (bool(params & COLORTINT_MASK))	postprocess = colortint(postprocess);
	

	ocolor = mix(basecolor, postprocess, blend);
}
