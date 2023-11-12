#version 430


# define INVERT_MASK		(1 << 0)
# define GRAYSCALE_MASK		(1 << 1)
# define GRAIN_MASK			(1 << 2)
# define SCANLINE_MASK		(1 << 3)
# define BLURRY_MASK		(1 << 4)
# define COLORTINT_MASK		(1 << 5)
# define CP_MASK		    (1 << 6)


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


// COLORTINT MASK
vec4 colortint(in vec4 color)
{
    return vec4(vec3(rTint, gTint, bTint) * color.rgb, color.a);
}


// GRAIN MASK
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

// SCAN LINE MASK
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

// BLURRY MASK
vec4 blurry(in vec4 color)
{
    float blurRadius = 5.0;
    vec2 texelSize = 1.0 / textureSize(screenTexture, 0);

    vec4 blurredColor = vec4(0.0);
    for (float i = -blurRadius; i <= blurRadius; i++)
    {
        for (float j = -blurRadius; j <= blurRadius; j++)
        {
            vec2 offset = vec2(i, j) * texelSize;
            blurredColor += texture(screenTexture, ftexcoord + offset);
        }
    }
    blurredColor /= pow(2.0 * blurRadius + 1.0, 2.0);

    return mix(color, blurredColor, blend);
}

// ITS GIVING CYBERPUNK MASK
vec4 itsGivingCyberPunk(in vec4 Color)
{
    float cpBlurStrength = 500.0;
    vec2 cpTexelSize = 1.0 / textureSize(screenTexture, 0);

    vec4 cpBlurredColor = vec4(0.0);

    for (float cpI = -1.0; cpI <= 1.0; cpI++)
    {
        for (float cpJ = -1.0; cpJ <= 1.0; cpJ++)
        {
            vec2 cpOffset = vec2(cpI, cpJ) * cpTexelSize;
            cpBlurredColor += texture(screenTexture, ftexcoord + cpOffset);
        }
    }
    cpBlurredColor /= 9.0;

    return mix(Color, cpBlurredColor, cpBlurStrength);
}



void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;

	if (bool(params & INVERT_MASK))		postprocess = invert(postprocess);
	if (bool(params & GRAYSCALE_MASK))	postprocess = grayscale(postprocess);
	if (bool(params & GRAIN_MASK))		postprocess = grain(postprocess);
	if (bool(params & SCANLINE_MASK))	postprocess = scanline(postprocess);
	if (bool(params & BLURRY_MASK))		postprocess = blurry(postprocess);
	if (bool(params & COLORTINT_MASK))	postprocess = colortint(postprocess);
	if (bool(params & CP_MASK))	        postprocess = itsGivingCyberPunk(postprocess);
	
    ocolor = mix(basecolor, postprocess, blend);
}
