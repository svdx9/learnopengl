#version 330 core
in vec3 vert_color;
out vec4 frag_color;

uniform vec4 ourColor; // we set this variable in the OpenGL code.

void main()
{
    // frag_color = vec4(vert_color, 1.0f);
    frag_color = ourColor;
}
