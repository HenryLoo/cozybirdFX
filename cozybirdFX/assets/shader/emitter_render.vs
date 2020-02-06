#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 2) in vec3 aColour;
layout (location = 3) in float aDuration;
layout (location = 4) in float aSize;
layout (location = 5) in int aType;

out vec3 vsColour;
out float vsDuration;
out float vsSize;
out int vsType;

void main()
{
    gl_Position = vec4(aPosition, 0.0, 1.0);

    // Simply pass all attributes to the geometry shader.
    vsColour = aColour;
    vsDuration = aDuration;
    vsSize = aSize;
    vsType = aType;
}
