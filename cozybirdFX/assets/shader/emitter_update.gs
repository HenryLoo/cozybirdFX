#version 330 core

// Treat input and ouput vertices as points.
// Each point is a particle.
layout(points) in;
layout(points) out;
layout(max_vertices = 50) out;

// Particle attributes from the vertex shader.
in vec2 vsPosition[];
in vec2 vsVelocity[];
in vec3 vsColour[];
in float vsDuration[];
in float vsSize[];
in int vsType[];

// Particle attributes to output for transform feedback.
out vec2 gsPosition;
out vec2 gsVelocity;
out vec3 gsColour;
out float gsDuration;
out float gsSize;
out int gsType;

// Emitter properties.
uniform vec2 emPos;
uniform vec2 emVelocityMin;
uniform vec2 emVelocityOffset;
uniform vec2 emAcceleration;
uniform vec3 emColour;
uniform float emDurationMin;
uniform float emDurationOffset;
uniform float emSize;
uniform int emNumToGenerate;

uniform float deltaTime;

uniform float randomSeed;
float localSeed;

// Rand and noise functions from:
// https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float rand(float n)
{
    return fract(sin(n) * 43758.5453123);
}

float noise()
{
	float fl = floor(localSeed);
    float fc = fract(localSeed);

    // Update the local seed so that the next call within this iteration
    // will have different results.
    localSeed = localSeed * rand(randomSeed) + 43758.5453123 * rand(localSeed);
	return mix(rand(fl), rand(fl + 1.0), fc);
}

void main()
{
    // Store a writable copy of the seed.
    localSeed = randomSeed;

    // Set output values.
    gsPosition = vsPosition[0];
    gsVelocity = vsVelocity[0];

    // If this is a particle, update its position and velocity.
    if (vsType[0] != 0)
    {
        gsPosition += gsVelocity * deltaTime;
        gsVelocity += emAcceleration * deltaTime;
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
            vec2 velOffset = vec2(
                emVelocityOffset.x * noise(),
                emVelocityOffset.y * noise()
            );
            gsVelocity = emVelocityMin + velOffset;
            gsColour = emColour;
            gsDuration = emDurationMin + emDurationOffset * noise();
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
