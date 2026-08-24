#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec3 a_normal;
out vec3 vertex_color;
out vec3 frag_pos;
out vec3 normal;
out vec4 frag_pos_light_space;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_space_matrix;

void main() {
    frag_pos = vec3(model * vec4(a_pos, 1.0));
    normal = mat3(transpose(inverse(model))) * a_normal;
    frag_pos_light_space = light_space_matrix * vec4(frag_pos, 1.0);
    gl_Position = projection * view * vec4(frag_pos, 1.0);
    vertex_color = a_color;
}