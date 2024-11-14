#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;

out vec3 outColor;		// fragment shader�� ���!�� �� �� �ְ� ���ش�!

//uniform vec3 uColor;
uniform mat4 modelTransform;	// C++ ���α׷�(CPU)�� ���(���� �ְ���� �� �ִ�) CPU���α׷��� �������� �������� ���� �ְ�޴´�
uniform mat4 viewTransform;
uniform mat4 projTransform;

void main()
{
	gl_Position = projTransform * viewTransform * modelTransform * vec4(vPos, 1.f);
	outColor = vColor;
}
