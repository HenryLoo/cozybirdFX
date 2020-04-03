#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aSpeedDirection;
layout (location = 2) in vec4 aRotationSizeLife;
layout (location = 3) in int aType;

out vec2 vsPosition;
out vec2 vsSpeedDirection;
out vec4 vsRotationSizeLife;
out int vsType;

void main()
{
    // Simply pass all attributes to the geometry shader.
    vsPosition = aPosition;
    vsSpeedDirection = aSpeedDirection;
    vsRotationSizeLife = aRotationSizeLife;
    vsType = aType;
}
