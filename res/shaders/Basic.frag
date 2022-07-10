#version 410 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
flat in float v_TexID;

uniform sampler2D u_Textures[32];

void main() {
    int index = int(v_TexID);
    color = texture(u_Textures[index], v_TexCoord) * v_Color;
}