#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in vec3 normal[3];

uniform mat4 projection;

const float LENGTH = 0.2;

void main()
{
    for (int i = 0; i < 3; i++) {

        // normal vertex
        gl_Position = projection * gl_in[i].gl_Position;
        EmitVertex();

        gl_Position = projection * (gl_in[i].gl_Position + vec4(normal[i], 0.0) * LENGTH);
        EmitVertex();

        EndPrimitive();
    }
} 