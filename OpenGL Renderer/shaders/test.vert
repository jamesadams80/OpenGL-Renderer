#version 400

layout( location = 0 ) in vec3 in_Vertex;
layout( location = 1 ) in vec2 in_TexCoord;
layout( location = 2 ) in vec3 in_NormCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec3 normal;

void main()
{
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * vec4(in_Vertex,1.0f);
    normal = in_NormCoord;
}