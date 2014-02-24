#version 400

layout( location = 0 ) in vec3 in_Vertex;
layout( location = 1 ) in vec2 in_TexCoord;
layout( location = 2 ) in vec3 in_NormCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

out vec3 normal;
out vec2 texCoord;
out vec3 eyeSpaceCoord;

void main()
{
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * vec4(in_Vertex,1.0f);
    normal = normalize(vec3(NormalMatrix * vec4(in_NormCoord,1.0f)));
    texCoord = in_TexCoord;
    eyeSpaceCoord = vec3( (ModelMatrix * ViewMatrix  ) * vec4(in_Vertex,1.0f));
}