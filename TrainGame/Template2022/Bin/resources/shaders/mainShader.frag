#version 400 core

// Structure holding light information:  its position as well as ambient, diffuse, and specular colours
struct LightInfo
{
vec4 position;
vec3 La;
vec3 Ld;
vec3 Ls;
vec3 direction;
float exponent;
float cutoff;
};

// Structure holding material information:  its ambient, diffuse, and specular colours, and shininess
struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};

// Lights and materials passed in as uniform variables from client programme
uniform LightInfo light1;
uniform LightInfo light2;
uniform MaterialInfo material1; 

//in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader
in vec4 p;
in vec3 n;

out vec4 vOutputColour;		// The output colour

uniform sampler2D sampler0;  // The texture sampler
uniform samplerCube CubeMapTex;
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform bool renderSkybox;
in vec3 worldPosition;
uniform float t;
uniform bool checkdiscard = false;

// This function implements the Phong shading model
// The code is based on the OpenGL 4.0 Shading Language Cookbook, Chapter 2, pp. 62 - 63, with a few tweaks. 
// Please see Chapter 2 of the book for a detailed discussion.
vec3 PhongModel(vec4 eyePosition, vec3 eyeNorm)
{
	vec3 s = normalize(vec3(light1.position - eyePosition));
	vec3 v = normalize(-eyePosition.xyz);
	vec3 r = reflect(-s, eyeNorm);
	vec3 n = eyeNorm;
	vec3 ambient = light1.La * material1.Ma;
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = light1.Ld * material1.Md * sDotN;
	vec3 specular = vec3(0.0f);
	float eps = 0.000001f; // add eps to shininess below -- pow not defined if second argument is 0 (as described in GLSL documentation)
	if (sDotN > 0.0f) 
		specular = light1.Ls * material1.Ms * pow(max(dot(r, v), 0.0f), material1.shininess + eps);
	return ambient + diffuse + specular;
}

//Implementing spotlight
vec3 BlinnPhongSpotlightModel(vec4 p, vec3 n)
{
	vec3 s = normalize(vec3(light2.position - p));
	float angle = acos(dot(-s, light2.direction));
	float cutoff = radians(clamp(light2.cutoff, 0.0, 90.0));
	vec3 ambient = light2.La * material1.Ma;
	if (angle < cutoff) {	
		float spotFactor = pow(dot(-s, light2.direction), light2.exponent);
		vec3 v = normalize(-p.xyz);
		vec3 h = normalize(v + s);
		float sDotN = max(dot(s, n), 0.0);
		vec3 diffuse = light2.Ld * material1.Md * sDotN;
		vec3 specular = vec3(0.0);
		if (sDotN > 0.0)
			specular = light2.Ls * material1.Ms * pow(max(dot(h, n), 0.0), material1.shininess);
			return ambient + spotFactor * (diffuse + specular);
	} else
		return ambient;
}

void main()
{
	if (renderSkybox) {
		vOutputColour = texture(CubeMapTex, worldPosition);

	} else {

		// Get the texel colour from the texture sampler
		vec4 vTexColour = texture(sampler0, vTexCoord);	
		float averageColour = (vTexColour.r + vTexColour.g + vTexColour.b)/3;
		if (averageColour < fract(t/5) && checkdiscard)
		discard;

		vec3 Colour = BlinnPhongSpotlightModel(p, normalize(n));
		Colour += BlinnPhongSpotlightModel(p, normalize(n));
		if (bUseTexture)
			vOutputColour = vTexColour*vec4(Colour, 1.0f);	// Combine object colour and texture 
		else
			vOutputColour = vec4(Colour, 1.0f);	// Just use the colour instead
	}
}

