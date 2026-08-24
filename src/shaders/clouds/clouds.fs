#version 330 core
in vec3 frag_pos;
out vec4 frag_color;
uniform vec3 view_pos;
uniform float fog_density;

void main() {
    vec3 cloud_color = vec3(1.0f, 1.0f, 1.0f);

    float dist = length(view_pos - frag_pos);

    float fog_start = 50.0;
    float fog_end = 75.0;

    float fog_factor = 1.0;
    if (dist > fog_start) {
        fog_factor = (fog_end - dist) / (fog_end - fog_start);
    }
    fog_factor = clamp(fog_factor, 0.0, 1.0);

    vec3 fog_color = vec3(0.53f, 0.81f, 0.92f);
    vec3 final_color = mix(fog_color, cloud_color, fog_factor);

    float alpha = 0.85f * fog_factor;
    frag_color = vec4(final_color, alpha);
}