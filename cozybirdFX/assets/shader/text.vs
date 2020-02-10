#version 330 core
layout (location = 0) in vec4 vertex;
layout (location = 1) in vec4 instanceClip;
layout (location = 2) in vec4 instanceColour;
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoords;
out vec4 colours;

uniform mat4 projection;

void main()
{
    gl_Position = projection * instanceMatrix * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vec2(vertex.z * instanceClip.z + instanceClip.x, vertex.w * instanceClip.w + instanceClip.y);
    colours = instanceColour;
}
