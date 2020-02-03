#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aVelocity;
layout (location = 2) in vec3 aColour;
layout (location = 3) in float aDuration;
layout (location = 4) in float aSize;
layout (location = 5) in int aType;

out vec3 vsPosition;
out vec3 vsVelocity;
out vec3 vsColour;
out float vsDuration;
out float vsSize;
out int vsType;

void main()
{
    // Simply pass all attributes to the geometry shader.
    vsPosition = aPosition;
    vsVelocity = aVelocity;
    vsColour = aColour;
    vsDuration = aDuration;
    vsSize = aSize;
    vsType = aType;
}