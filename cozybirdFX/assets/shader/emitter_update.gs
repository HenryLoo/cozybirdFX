#version 330 core

// Treat input and ouput vertices as points.
// Each point is a particle.
layout(points) in;
layout(points) out;
layout(max_vertices = 50) out;

// Particle attributes from the vertex shader.
in vec2 vsPosition[];
in vec2 vsSpeedDirection[];
in vec4 vsRotationSizeLife[];
in int vsType[];

// Particle attributes to output for transform feedback.
out vec2 gsPosition;
out vec2 gsSpeedDirection;
out vec4 gsRotationSizeLife;
out int gsType;

// Emitter properties.
uniform vec2 emPos;
uniform vec3 emSpeed; // Min, max, growth
uniform vec3 emDirection; // Min, max, growth
uniform vec3 emRotation; // Min, max, growth
uniform vec2 emLife; // Min, max
uniform vec3 emSize; // Min, max, growth
uniform int emNumToGenerate;
uniform vec2 emDistribution; // Width, height
uniform bool emIsFacingDirection;

uniform float deltaTime;

uniform float randomSeed;
float localSeed;

uniform bool isClearParticles;

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
    gsSpeedDirection = vsSpeedDirection[0];
    gsRotationSizeLife = vsRotationSizeLife[0];
    gsType = vsType[0];

    // If this is a particle, update its position and velocity.
    if (gsType != 0)
    {
        // Get direction unit vector from rotating by direction.
        vec2 unitVec = vec2(1, 0);
        float dirAngle = gsSpeedDirection.y;
        vec2 direction = vec2(
            unitVec.x * cos(dirAngle) - unitVec.y * sin(dirAngle),
            unitVec.x * sin(dirAngle) + unitVec.y * cos(dirAngle)
        );

        // Update position.
        float speed = gsSpeedDirection.x;
        gsPosition.x += direction.x * speed * deltaTime;
        gsPosition.y += direction.y * speed * deltaTime;

        // Update speed.
        gsSpeedDirection.x += emSpeed.z * deltaTime;

        // Update direction.
        gsSpeedDirection.y += emDirection.z * deltaTime;

        // Face the direction of movement.
        // This overrides rotation settings.
        if (emIsFacingDirection)
        {
            gsRotationSizeLife.x = gsSpeedDirection.y;
        }
        else
        {
            // Update rotation.
            gsRotationSizeLife.x += emRotation.z * deltaTime;
        }

        // Update current life.
        gsRotationSizeLife.z -= deltaTime;

        // Update size.
        gsRotationSizeLife.y += emSize.z * deltaTime;
        if (gsRotationSizeLife.y < 0.0)
        {
            gsRotationSizeLife.y = 0.0;
        }

        // Otherwise, this is a particle. So just emit it.
        if (!isClearParticles && gsRotationSizeLife.z > 0.0)
        {
            EmitVertex();
            EndPrimitive();
        }
    }
    // If this is an emitter, generate its particles.
    else if (gsType == 0)
    {
        // Output the vertex corresponding to the emitter so we can begin
        // generating new points for its particles.
        EmitVertex();
        EndPrimitive();

        // Don't generate particles if clearing.
        if (isClearParticles)
            return;

        for (int i = 0; i < emNumToGenerate; ++i)
        {
            // Set the new particle attributes from emitter properties.
            gsPosition.x = emPos.x - emDistribution.x + emDistribution.x * 2 * noise();
            gsPosition.y = emPos.y - emDistribution.y + emDistribution.y * 2 * noise();

            float speedOffset = emSpeed.y - emSpeed.x;
            gsSpeedDirection.x = emSpeed.x + speedOffset * noise();

            float directionOffset = emDirection.y - emDirection.x;
            gsSpeedDirection.y = emDirection.x + directionOffset * noise();

            // Face the direction of movement.
            // This overrides rotation settings.
            if (emIsFacingDirection)
            {
                gsRotationSizeLife.x = gsSpeedDirection.y;
            }
            else
            {
                float rotationOffset = emRotation.y - emRotation.x;
                gsRotationSizeLife.x = emRotation.x + rotationOffset * noise();
            }

            float sizeOffset = emSize.y - emSize.x;
            gsRotationSizeLife.y = emSize.x + sizeOffset * noise();

            float lifeOffset = emLife.y - emLife.x;
            gsRotationSizeLife.z = emLife.x + lifeOffset * noise();
            gsRotationSizeLife.w = gsRotationSizeLife.z;

            // Emit the particle.
            gsType = 1;
            EmitVertex();
            EndPrimitive();
        }
    }
}
