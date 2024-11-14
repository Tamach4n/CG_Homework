#version 330 core
//layout (location = 0) in vec2 vPos;

uniform vec4 startEndPos;

void main()
{
	//gl_Position = vec4(vPos, 0.f, 1.f);
	if (0 == gl_VertexID)
		gl_Position = vec4(startEndPos.x, startEndPos.y, 0.f, 1.f);
	
	else
		gl_Position = vec4(startEndPos.z, startEndPos.w, 0.f, 1.f);
}