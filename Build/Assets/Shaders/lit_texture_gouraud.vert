#version 430
 
// vertex position, texture coordinates, and vertex normal
in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;
 
 // output variables - position, normal texxture coordinates, and color
out layout(location = 0) vec3 oposition;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec2 otexcoord;
out layout(location = 3) vec4 ocolor;
 
// uniform variables for transformations - model matrix, view matrix, projection matrix
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
 
// material properties
uniform struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
 
	vec2 offset;
	vec2 tiling;
} material;
 

// light properties
uniform struct Light
{
	vec3 position;
	vec3 color;
} light;
 
// ambient light color
uniform vec3 ambientLight;
 

// calculating ambient, diffuse and specular lighting with GOURAUD method
vec3 ads(in vec3 position, in vec3 normal)
{
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
 
void main()
{
	mat4 modelView = view * model;
 
	// convert position and normal to world-view space - GOURAUD
	oposition = vec3(modelView * vec4(vposition, 1));
	onormal = normalize(mat3(modelView) * vnormal);
	otexcoord = (vtexcoord * material.tiling) + material.offset;
 
	// setting the final color
	ocolor = vec4(ads(oposition, onormal), 1);

	// transform vertex position to the space used for rendering
	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vposition, 1.0);
}