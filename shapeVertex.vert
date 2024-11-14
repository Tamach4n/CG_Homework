#version 330 core

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec3 vColor;

out vec3 outColor;		// fragment shader와 통신!을 할 수 있게 해준다!

//uniform vec3 uColor;
uniform vec2 pos;
/*
uniform float size;
uniform vec3 rectColor;

uniform float afterTime;

void main()
{
	float speed = 0.3f;

	vec2 pos[6];
	pos[0] = vec2(size, size) + afterTime * vec2(-1.f, -1.f) * speed;
	pos[1] = vec2(-size, size) + afterTime * vec2(-1.f, -1.f) * speed;
	pos[2] = vec2(-size, -size) + afterTime * vec2(-1.f, -1.f) * speed;
	pos[3] = vec2(-size, -size) + afterTime * vec2(1.f, -1.f) * speed;
	pos[4] = vec2(size, -size) + afterTime * vec2(1.f, -1.f) * speed;
	pos[5] = vec2(size, size) + afterTime * vec2(1.f, -1.f) * speed;

	gl_Position = modelTransform * vec4(pos[gl_VertexID], 0.f, 1.f);
	outColor = rectColor;
}*/


void main()
{
	gl_Position = vec4(pos + vPos, 0.f, 1.f);
	outColor = vColor;
}

