#version 330 core
in vec2 TexCoords;
in vec4 colours;

out vec4 FragColour;

uniform sampler2D text;

void main()
{
    if (TexCoords.x <= 0.01 || TexCoords.x >= 0.99 ||
        TexCoords.y <= 0.01 || TexCoords.y >= 0.99)
    {
        FragColour = vec4(1.0, 1.0, 1.0, 1.0);
        return;
    }

    FragColour = colours;
}
