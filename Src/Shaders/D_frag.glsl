#version 410 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

in vec4 vertexColor;

void main()
{
	color = u_Color * 0 + vertexColor;
};