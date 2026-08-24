#version 330 core
in vec3 vertex_color;
in vec3 frag_pos;
in vec3 normal;
in vec4 frag_pos_light_space;
out vec4 frag_color;

uniform sampler2D shadow_map;
uniform vec3 light_dir;
uniform vec3 view_pos;
uniform float fog_density;

float calculate_shadow(vec4 light_space_coord) {
    vec3 proj_coords = light_space_coord.xyz / light_space_coord.w;
    proj_coords = proj_coords * 0.5 + 0.5;
    if(proj_coords.z > 1.0) return 0.0;
    float closest_depth = texture(shadow_map, proj_coords.xy).r;
    float current_depth = proj_coords.z;
    float bias = max(0.005 * (1.0 - dot(normal, -light_dir)), 0.0005);
    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
           float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
           shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}

void main() {
    vec3 light_color = vec3(1.0, 0.95, 0.85);
    vec3 ambient = 0.35 * vertex_color;
    float diff = max(dot(normalize(normal), -light_dir), 0.0);
    vec3 diffuse = diff * light_color * vertex_color;
    float shadow = calculate_shadow(frag_pos_light_space);
    vec3 lighting = (ambient + (1.0 - shadow) * diffuse);

    float dist = length(view_pos - frag_pos);

    float fog_start = 50.0;
    float fog_end = 75.0;

    float fog_factor = 1.0;
    if (dist > fog_start) {
        fog_factor = (fog_end - dist) / (fog_end - fog_start);
    }
    fog_factor = clamp(fog_factor, 0.0, 1.0);

    vec3 fog_color = vec3(0.53f, 0.81f, 0.92f);
    vec3 final_color = mix(fog_color, lighting, fog_factor);

    frag_color = vec4(final_color, 1.0f);
}