#version 330

layout (location=0) in vec4 Surface;
layout (location=1) in vec3 Normals;

out VS_OUT {
    out vec3 N;
    out vec3 L;
    out vec3 V;
    out vec4 distance;
    out float amplitude;
    out float wake;
} vs_out;


uniform vec4 Offset;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform vec3 CameraPosition;

void main() {
    // distance from camera
    vs_out.distance = View * Model * vec4(Surface.xyz, 1.0);
    // vertex in world coords
    vec3 worldcoords = (View * Model * vec4(Surface.xyz, 1.0)).xyz;
    // normal in world coords
    vs_out.N = normalize((inverse(transpose(Model)) * vec4(Normals, 1.0)).xyz);
    // diffuse 
    vs_out.L = normalize(LightPosition.xyz - worldcoords);
    // specular 
    vs_out.V = normalize(-worldcoords);
    // wave amplitude
    vs_out.amplitude = Surface.y;
    // wake position
    vs_out.wake = Surface.w;

    gl_Position = Projection * View * Model * (Surface + Offset);
}
