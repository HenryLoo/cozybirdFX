#version 330

uniform sampler2D textureSampler;

smooth in vec2 texCoord;
flat in vec4 particleColour;
flat in float particleAdditivity;

out vec4 FragColour;

void main()
{
    vec4 textureColour = texture2D(textureSampler, texCoord);
    if (textureColour.a == 0.0)
        discard;

    vec4 colour = vec4(textureColour.rgb * particleColour.rgb, textureColour.a);

    // Premultiplied alpha.
    colour = vec4(colour.rgb * colour.a, colour.a);

    // Adjust blend mode.
    colour.a *= (1.0 - particleAdditivity);

    // Multiply by opacity.
    colour *= particleColour.a;

    FragColour = colour;
}
