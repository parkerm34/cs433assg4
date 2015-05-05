#version 410 core

uniform mat3 NormalMatrix;
uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;

out vec3 color;
out vec4 Position;
out vec3 Normal;

void
main()
{
	Normal = normalize(NormalMatrix * vNormal);
	Position = MVMatrix * vec4(VertexPosition, 1.0f);
	gl_position = MVPMatrix * vec4(VertexPosition, 1.0f);

	color = vColor;
}
