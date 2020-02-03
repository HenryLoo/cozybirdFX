#version 330

uniform sampler2D textureSampler;

smooth in vec2 texCoord;
flat in vec4 particleColour;

out vec4 FragColour;

void main()
{
    vec4 textureColour = texture2D(textureSampler, texCoord);
    if(textureColour.a == 0.0)
        discard;

    FragColour = vec4(textureColour.xyz, 1.0) * particleColour;
}
