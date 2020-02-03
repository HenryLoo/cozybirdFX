#version 330 core

// Treat input and ouput vertices as points.
// Each point is a particle.
layout(points) in;
layout(points) out;
layout(max_vertices = 50) out;

// Particle attributes from the vertex shader.
in vec3 vsPosition[];
in vec3 vsVelocity[];
in vec3 vsColour[];
in float vsDuration[];
in float vsSize[];
in int vsType[];

// Particle attributes to output for transform feedback.
out vec3 gsPosition;
out vec3 gsVelocity;
out vec3 gsColour;
out float gsDuration;
out float gsSize;
out int gsType;

// Emitter properties.
uniform vec3 emPos;
uniform vec3 emVelocityMin;
uniform vec3 emVelocityOffset;
uniform vec3 emColour;
uniform float emDuration;
uniform float emSize;
uniform int emNumToGenerate;

uniform vec3 gravityVector;
uniform float deltaTime;

void main()
{
    // Set output values.
    gsPosition = vsPosition[0];
    gsVelocity = vsVelocity[0];

    // If this is a particle, update its position and velocity.
    if (vsType[0] != 0)
    {
        gsPosition += gsVelocity * deltaTime;
        gsVelocity += gravityVector * deltaTime;
    }

    gsColour = vsColour[0];
    gsDuration = vsDuration[0] - deltaTime;
    gsSize = vsSize[0];
    gsType = vsType[0];

    // If this is an emitter, generate its particles.
    if (gsType == 0)
    {
        // Output the vertex corresponding to the emitter so we can begin
        // generating new points for its particles.
        EmitVertex();
        EndPrimitive();

        for (int i = 0; i < emNumToGenerate; ++i)
        {
            // Set the new particle attributes from emitter properties.
            gsPosition = emPos;
            vec3 velOffset = vec3(
                emVelocityOffset.x,
                emVelocityOffset.y,
                emVelocityOffset.z
            );
            gsVelocity = emVelocityMin + velOffset;
            gsColour = emColour;
            gsDuration = emDuration;
            gsSize = emSize;

            // Emit the particle.
            gsType = 1;
            EmitVertex();
            EndPrimitive();
        }
    }
    // Otherwise, this is a particle. So just emit it.
    else if (gsDuration > 0.0)
    {
        EmitVertex();
        EndPrimitive();
    }
}
