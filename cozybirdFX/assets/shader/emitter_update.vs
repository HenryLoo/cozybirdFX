#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aVelocity;
layout (location = 2) in float aCurrentLife;
layout (location = 3) in float aLife;
layout (location = 4) in float aSize;
layout (location = 5) in int aType;

out vec2 vsPosition;
out vec2 vsVelocity;
out float vsCurrentLife;
out float vsLife;
out float vsSize;
out int vsType;

void main()
{
    // Simply pass all attributes to the geometry shader.
    vsPosition = aPosition;
    vsVelocity = aVelocity;
    vsCurrentLife = aCurrentLife;
    vsLife = aLife;
    vsSize = aSize;
    vsType = aType;
}
