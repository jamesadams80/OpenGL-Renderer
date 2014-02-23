#version 400

in vec3 normal;
in vec2 texCoord;
in vec4 lightVector;

uniform sampler2D ambientTexture;

// Material Ambient, diffuse and specular components
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

struct LightInfo
{
    vec3 position;
    vec3 ambientColour;
    vec3 diffuseColour;
    vec3 specularColour;
    float specularExponent;
};

uniform LightInfo light;

void main()
{
    vec3 lightDirection = normalize(light.position - lightVector.xyz);

    vec3 ambientColour =  light.ambientColour *  Ka * texture( ambientTexture, vec2( texCoord.s, -texCoord.t )).rgb;

    vec3 diffuseColour = (light.diffuseColour * Kd* texture( ambientTexture, vec2( texCoord.s, -texCoord.t )).rgb) * max( dot(normal,lightDirection),0.0);

	gl_FragColor =  vec4( ambientColour + diffuseColour, 1.0f);
}