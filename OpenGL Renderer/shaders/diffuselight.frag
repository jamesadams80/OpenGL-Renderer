#version 400

in vec3 normal;
in vec2 texCoord;
in vec3 lightVector;

uniform sampler2D diffuseTexture;

// Material Ambient, diffuse and specular components
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;

const int MAX_NUMBER_OF_LIGHTS = 8;

uniform vec3 lightPositions[MAX_NUMBER_LIGHTS];
uniform vec3 lightAmbientComp[MAX_NUMBER_LIGHTS];
uniform vec3 lightDiffuseComp[MAX_NUMBER_LIGHTS];
uniform vec3 lightSpecularComp[MAX_NUMBER_LIGHTS];

void main()
{
    vec3 lightDirection = normalize( light.position - lightVector );

    vec3 reflectionVec = normalize( -reflect( lightDirection, normal ) );

    vec3 eyePosition = normalize( -lightVector );

    vec3 ambientColour = light.ambientColour * Ka;

    vec3 diffuseColour = (light.diffuseColour * Kd * texture( diffuseTexture, vec2( texCoord.s, -texCoord.t )).rgb) * max( dot(normal,lightDirection),0.0);

    vec3 specularColour =  vec3(0.5f,0.5f,0.5f) * vec3(0.5f,0.5f,0.5f) * ( pow ( max ( 0.0f, dot( reflectionVec,eyePosition ) ) , 14.0f) );

	gl_FragColor =  vec4( ambientColour + diffuseColour + specularColour, 1.0f);
}