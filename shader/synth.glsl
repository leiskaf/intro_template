#version 450

//#define SAMPLE_RATE (44100.0)
#define PI (3.14159)
//#define BUF_WIDTH 1024

layout(local_size_x=1)in int compute_local_size;
buffer b{vec2 o[1024];};
uniform float T; 

void main() {
    float t = float(gl_WorkGroupID.x) + T;

    float mono = 0.1*sin( 2.0*PI*440.0*t/44100);
    vec2 wave = vec2(mono, mono);
    o[gl_WorkGroupID.x] = wave;
}
