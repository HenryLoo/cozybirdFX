#version 330

// Create quads for each point.
layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

in vec3 vsColour[];
in float vsDuration[];
in float vsSize[];
in int vsType[];

smooth out vec2 texCoord;
flat out vec4 particleColour;

uniform mat4 mvp;

// Two unit vectors, used as axes to span the quad.
uniform vec3 axis1;
uniform vec3 axis2;

void main()
{
    // Skip this if it is an emitter.
    if (vsType[0] == 0)
        return;

    vec3 origin = gl_in[0].gl_Position.xyz;

    particleColour = vec4(vsColour[0], vsDuration[0]);

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
