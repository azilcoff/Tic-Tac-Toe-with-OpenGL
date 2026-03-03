#version 330 core

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec3 vColor;

out vec4 outVColor;

void main()
{
    gl_Position = vec4(vPos.x, vPos.y, 0.0f, 1.0f);
    outVColor = vec4(vColor, 1.0f);
}