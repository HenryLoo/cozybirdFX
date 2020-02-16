#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec3 border;
layout (location = 2) in vec4 instanceColour;
layout (location = 3) in mat4 instanceMatrix;

out vec2 position;
out vec3 uiBorder;
out vec4 colours;

uniform mat4 projection;

void main()
{
    gl_Position = projection * instanceMatrix * vec4(vertex.xy, 0.0, 1.0);
    position = vertex;
    uiBorder = border;
    colours = instanceColour;
}
