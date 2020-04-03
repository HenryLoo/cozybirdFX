#version 330 core
in vec2 TexCoords;
in vec4 colours;

out vec4 FragColour;

uniform sampler2D text;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    FragColour = colours * sampled;
}
