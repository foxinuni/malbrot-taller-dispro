#version 330 core

out vec4 FragColor;
in vec2 position;

uniform vec2 u_resolution;
uniform vec2 u_center;
uniform float u_zoom;

vec4 iteration_to_color(int iteration, int max_iteration) {
    float t = float(iteration) / float(max_iteration);
    if (t >= 1.0) {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }

    float r = float(9 * (1 - t) * t * t * t);
    float g = float(15 * (1 - t) * (1 - t) * t * t);
    float b = float(8.5 * (1 - t) * (1 - t) * (1 - t) * t);
   
    return vec4(r, g, b, 1.0);
}

vec2 position_to_complex(vec2 position, vec2 resolution, vec2 center, float zoom, vec2 min, vec2 max) {
    float aspect = resolution.x / resolution.y;
    vec2 normalized = ((position + vec2(1.0)) / 2.0) * (vec2(2.0 * aspect, 2.0) / zoom) + center - vec2(aspect, 1.0) / zoom;
    return normalized;
}

void main() {
    vec2 c = position_to_complex(position, u_resolution, u_center, u_zoom, vec2(-2.0, 1.0), vec2(-1.0, 1.0));
    vec2 z = c;
    int n = 0;

    for (int i = 0; i < 250; i++) {
        if (length(z) > 2.0) break;
        z = vec2(z.x*z.x - z.y*z.y, 2.0 * z.x * z.y) + c;
        n += 1;
    }

    FragColor = iteration_to_color(n, 250);
}