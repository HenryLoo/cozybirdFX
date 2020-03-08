#version 330

// Create quads for each point.
layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

in float vsCurrentLife[];
in float vsLife[];
in float vsSize[];
in int vsType[];

smooth out vec2 texCoord;
flat out vec4 particleColour;

uniform mat4 mvp;
uniform vec4 colour;
uniform vec4 birthColour;
uniform vec4 deathColour;
uniform float duration;

// Two unit vectors, used as axes to span the quad.
uniform vec3 axis1;
uniform vec3 axis2;

void main()
{
    // Skip this if it is an emitter.
    if (vsType[0] == 0)
        return;

    vec3 origin = gl_in[0].gl_Position.xyz;

    //particleColour = vec4(vsColour[0], vsDuration[0]);
    float dur = (vsLife[0] - vsCurrentLife[0]) / vsLife[0];
    float birthThreshold = 0.33;
    float deathThreshold = 0.66;
    if (dur <= birthThreshold)
        particleColour = mix(birthColour, colour, dur / birthThreshold);
    else if (dur >= deathThreshold)
        particleColour = mix(colour, deathColour, (dur - deathThreshold) / (1.0 - deathThreshold));
    else
        particleColour = colour;

    // Define quad vertices.
    vec3 position = origin + (-axis1 - axis2) * vsSize[0];
    texCoord = vec2(0.0, 1.0);
    gl_Position = mvp * vec4(position, 1.0);
    EmitVertex();

    position = origin + (-axis1 + axis2) * vsSize[0];
    texCoord = vec2(0.0, 0.0);
    gl_Position = mvp * vec4(position, 1.0);
    EmitVertex();

    position = origin + (axis1 - axis2) * vsSize[0];
    texCoord = vec2(1.0, 1.0);
    gl_Position = mvp * vec4(position, 1.0);
    EmitVertex();

    position = origin + (axis1 + axis2) * vsSize[0];
    texCoord = vec2(1.0, 0.0);
    gl_Position = mvp * vec4(position, 1.0);
    EmitVertex();

    EndPrimitive();
}
