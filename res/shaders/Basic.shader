#shader vertex
#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in float texID;

uniform mat4 u_MVP;

out vec2 v_TexCoord; 
out vec4 v_Color;
flat out float v_TexID;

void main() {
    v_Color = color;
    v_TexCoord = texCoord;
    v_TexID = texID;
    gl_Position =  u_MVP * position;
}

#shader fragment
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