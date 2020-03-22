#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 2) in vec4 aRotationSizeLife;
layout (location = 3) in int aType;

out vec4 vsRotationSizeLife;
out int vsType;

void main()
{
    gl_Position = vec4(aPosition, 0.0, 1.0);

    // Simply pass all attributes to the geometry shader.
    vsRotationSizeLife = aRotationSizeLife;
    vsType = aType;
}
