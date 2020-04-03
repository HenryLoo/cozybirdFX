#version 330

// Create quads for each point.
layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

in vec4 vsRotationSizeLife[];
in int vsType[];

smooth out vec2 texCoord;
flat out vec4 particleColour;
flat out float particleAdditivity;

uniform mat4 mvp;
uniform vec4 colour;
uniform vec4 birthColour;
uniform vec4 deathColour;
uniform float duration;
uniform float additivity;

// Two unit vectors, used as axes to span the quad.
uniform vec3 axis1;
uniform vec3 axis2;

vec2 rotatePoint(vec2 point, float radians)
{
    return vec2(
        point.x * cos(radians) - point.y * sin(radians),
        point.x * sin(radians) + point.y * cos(radians)
    );
}

vec4 getPosition(vec3 position, vec3 origin)
{
    float rotation = vsRotationSizeLife[0].x;
    vec2 point = rotatePoint(position.xy, rotation);
    return mvp * vec4(origin.xy + point, origin.z, 1.0);
}

void main()
{
    // Skip this if it is an emitter.
    if (vsType[0] == 0)
        return;

    vec3 origin = gl_in[0].gl_Position.xyz;

    // Blend colour, birth colour, and death colour based on the particle's
    // current life.
    float currentLife = vsRotationSizeLife[0].z;
    float totalLife = vsRotationSizeLife[0].w;
    float dur = (totalLife - currentLife) / totalLife;
    float birthThreshold = 0.33;
    float deathThreshold = 0.66;
    if (dur <= birthThreshold)
    {
        float amount = dur / birthThreshold;
        particleColour = mix(birthColour, colour, amount);
    }
    else if (dur >= deathThreshold)
    {
        float amount = (dur - deathThreshold) / (1.0 - deathThreshold);
        particleColour = mix(colour, deathColour, amount);
    }
    else
    {
        particleColour = colour;
    }

    particleAdditivity = additivity;

    // Define quad vertices.
    float size = vsRotationSizeLife[0].y;

    vec3 position = (-axis1 - axis2) * size;
    gl_Position = getPosition(position, origin);
    texCoord = vec2(0.0, 1.0);
    EmitVertex();

    position = (-axis1 + axis2) * size;
    gl_Position = getPosition(position, origin);
    texCoord = vec2(0.0, 0.0);
    EmitVertex();

    position = (axis1 - axis2) * size;
    texCoord = vec2(1.0, 1.0);
    gl_Position = getPosition(position, origin);
    EmitVertex();

    position = (axis1 + axis2) * size;
    texCoord = vec2(1.0, 0.0);
    gl_Position = getPosition(position, origin);
    EmitVertex();

    EndPrimitive();
}
