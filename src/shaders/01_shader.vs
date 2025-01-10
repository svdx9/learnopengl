
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
out vec3 vert_color;
uniform vec3 offset; // we set this variable in the OpenGL code.
void main()
{
    gl_Position = vec4(pos.x+offset.x, pos.y+offset.y, pos.z+offset.z, 1.0);
    vert_color = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
}
