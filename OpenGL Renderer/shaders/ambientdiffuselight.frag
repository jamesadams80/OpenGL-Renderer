#version 400

in vec3 normal;
in vec2 texCoord;
in vec3 eyeSpaceCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;

// Material Ambient, diffuse and specular components
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;

const int MAX_NUMBER_OF_LIGHTS = 8;
const float ATTENUATION_FACTOR = 0.0001;

uniform vec3 lightPositions[MAX_NUMBER_OF_LIGHTS];
uniform vec3 lightAmbientComp[MAX_NUMBER_OF_LIGHTS];
uniform vec3 lightDiffuseComp[MAX_NUMBER_OF_LIGHTS];
uniform vec3 lightSpecularComp[MAX_NUMBER_OF_LIGHTS];
uniform int numberOfLights;

void main()
{
    vec3 n = normalize(normal);

    vec3 totalAmbient;
    vec3 totalDiffuse;
    vec3 totalSpecular;

    for( int i = 0; i < numberOfLights; i++)
    {
        vec3 eyeLightPos = vec3( (ModelMatrix * ViewMatrix) * vec4(lightPositions[i],1.0f));

        vec3 lightDirection = normalize( eyeLightPos - eyeSpaceCoord );

        float distanceToLight = length(eyeLightPos - eyeSpaceCoord);

        float attenuation = 1 / (1 + ATTENUATION_FACTOR * pow(distanceToLight,2));

        vec3 reflectionVec =  - normalize( reflect( lightDirection, n ) );

        vec3 eyePosition = normalize( -eyeSpaceCoord );

        vec3 ambientColour = attenuation * lightAmbientComp[i] * Ka * texture( ambientTexture, vec2( texCoord.s, -texCoord.t )).rgb;

        float diffuseCoef =  max( dot(n,lightDirection),0.0f);

        vec3 diffuseColour = (attenuation * lightDiffuseComp[i] * Kd * texture( diffuseTexture, vec2( texCoord.s, -texCoord.t )).rgb) * diffuseCoef;

        vec3 specularColour = vec3(0.0f,0.0f,0.0f);

        if( diffuseCoef > 0 )
        {
            specularColour = clamp( attenuation * Ks * lightSpecularComp[i] * ( pow ( max ( dot( reflectionVec, eyePosition ), 0.0f ) , Ns) ), 0.0f, 1.0f);
        }

        totalAmbient += ambientColour;
        totalDiffuse += diffuseColour;
        totalSpecular += specularColour;
    }

    totalAmbient = clamp(totalAmbient,0.0f,1.0f);
    totalDiffuse = clamp(totalDiffuse,0.0f,1.0f);
    totalSpecular = clamp(totalSpecular,0.0f,1.0f);

	gl_FragColor =  vec4( totalAmbient + totalDiffuse + totalSpecular, 1.0f);
}
