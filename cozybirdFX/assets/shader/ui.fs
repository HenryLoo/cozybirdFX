#version 330 core
in vec2 position;
in vec3 uiBorder;
in vec4 colours;

out vec4 FragColour;

uniform sampler2D text;

void main()
{
    vec2 borderWidth = vec2( 1.0 ) / uiBorder.xy;
    bool hasBorder = (uiBorder.z == 1.0);
    if (hasBorder &&
        (position.x <= borderWidth.x || position.x >= (1.0 - borderWidth.x) ||
        position.y <= borderWidth.y || position.y >= (1.0 - borderWidth.y)))
    {
        FragColour = vec4(1.0, 1.0, 1.0, 1.0);
        return;
    }

    FragColour = colours;
}
