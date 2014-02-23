#version 400

in vec3 normal;
in vec2 texCoord;
in vec3 lightVector;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D alphaTexture;

// Material Ambient, diffuse and specular components
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;

const int MAX_NUMBER_OF_LIGHTS = 8;

uniform vec3 lightPositions[MAX_NUMBER_OF_LIGHTS];
uniform vec3 lightAmbientComp[MAX_NUMBER_OF_LIGHTS];
uniform vec3 lightDiffuseComp[MAX_NUMBER_OF_LIGHTS];
uniform vec3 lightSpecularComp[MAX_NUMBER_OF_LIGHTS];
uniform int numberOfLights;


void main()
{
    float alphaValue = texture( alphaTexture, vec2( texCoord.s, -texCoord.t )).r;

    if( alphaValue <= 0.2f )
    {
        discard;
    }

    vec3 n = normalize(normal);

    vec3 totalAmbient;
    vec3 totalDiffuse;
    vec3 totalSpecular;

    for( int i = 0; i < numberOfLights; i++)
    {
        vec3 eyeLightPos = vec3( (ModelMatrix * ViewMatrix) * vec4(lightPositions[i],1.0f));

        vec3 lightDirection = normalize( eyeLightPos - lightVector );

        vec3 reflectionVec =  - normalize( reflect( lightDirection, n ) );

        vec3 eyePosition = normalize( -lightVector );

        vec3 ambientColour = lightAmbientComp[i] * Ka * texture( ambientTexture, vec2( texCoord.s, -texCoord.t )).rgb;

        float diffuseCoef =  max( dot(n,lightDirection),0.0f);

        vec3 diffuseColour = (lightDiffuseComp[i] * Kd * texture( diffuseTexture, vec2( texCoord.s, -texCoord.t )).rgb) * diffuseCoef;

        vec3 specularColour = vec3(0.0f,0.0f,0.0f);

        if( diffuseCoef > 0 )
        {
            specularColour = clamp( Ks * lightSpecularComp[i] * ( pow ( max ( dot( reflectionVec, eyePosition ), 0.0f ) , Ns) ), 0.0f, 1.0f);
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
