#version 330

layout (location=0) in vec4 Position;
layout (location=1) in vec3 Normals;

out VS_OUT {
    out vec3 N;
    out vec3 L;
    out vec3 V;
    out vec4 distance;
    out float amplitude;
    out float wake;
} vs_out;


uniform vec3 Offset;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;

void main() {
    // distance from camera
    vs_out.distance = View * Model * vec4(Position.xyz, 1.0);
    // vertex in world coords
    vec3 worldcoords = (View * Model * vec4(Position.xyz, 1.0)).xyz;
    // normal in world coords
    vs_out.N = normalize((inverse(transpose(Model)) * vec4(Normals, 1.0)).xyz);
    // diffuse 
    vs_out.L = normalize(LightPosition - worldcoords);
    // specular 
    vs_out.V = normalize(-worldcoords);
    // wave amplitude
    vs_out.amplitude = Position.y;
    // wake position
    vs_out.wake = Position.w;

    gl_Position = Projection * View * Model * vec4(Position.xyz + Offset, 1.0);
}
