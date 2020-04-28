/* File generated with Shader Minifier 1.1.6
 * http://www.ctrl-alt-test.fr
 */
#ifndef SYNTH_H_
# define SYNTH_H_

const char *synth_glsl =
 "#version 450\n"
 "#define PI (3.14159)\n"
 "layout(local_size_x=1)in;buffer b{vec2 o[1024];};"
 "uniform float T;"
 "void main()"
 "{"
   "float P=float(gl_WorkGroupID.x)+T,x=.1*sin(2.*PI*440.*P/44100);"
   "vec2 f=vec2(x,x);"
   "o[gl_WorkGroupID.x]=f;"
 "}";

#endif // SYNTH_H_
