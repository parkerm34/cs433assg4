#version 410 core

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vKa;
layout(location = 3) in vec3 vKd;
layout(location = 4) in vec3 vKs;
layout(location = 5) in vec3 vS;
//layout(location = 6) in float s;

out vec3 ambient;
out vec3 diffuse;
out vec3 specular;
//out float shiniess;
out vec3 Normal;
out vec4 Position;

void
main()
{
	mat4 MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;
	mat4 MVMatrix = ViewMatrix * ModelMatrix;
	Normal = normalize(NormalMatrix * vNormal);
	Position = MVMatrix * vec4(vPosition, 1.0f);
	gl_Position = MVPMatrix * vec4(vPosition, 1.0f);
//	Position = ModelMatrix * ViewMatrix * vec4(vPosition, 1.0f);
//	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vPosition, 1.0f);
//	Position = ViewMatrix * ModelMatrix * vec4(vPosition, 1.0f);

	ambient = vKa;
	diffuse = vKd;
	specular = vKs;
//	shininess = s;
}
