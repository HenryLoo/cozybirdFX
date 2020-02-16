#version 330 core
out vec4 FragColour;

in vec2 TexCoord;

uniform sampler2D spriteTexture;

void main()
{
    vec4 spriteColour = texture(spriteTexture, TexCoord);
    if (spriteColour.a == 0.0)
        discard;

    FragColour = spriteColour;
}
