#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float interpolate_val;
void main() {
    // stbi_set_flip_vertically_on_load(true);
    // make the happy face looks in the other/reverse direction by changing the fragment shader:
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x * -1, TexCoord.y * -1)), 0.2);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), interpolate_val);
}
