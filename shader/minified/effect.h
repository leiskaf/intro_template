/* File generated with Shader Minifier 1.1.6
 * http://www.ctrl-alt-test.fr
 */
#ifndef EFFECT_H_
# define EFFECT_H_

const char *effect_glsl =
 "#version 450\n"
 "out vec4 fragColor;"
 "uniform float T=0.;"
 "float s=T;"
 "void main()"
 "{"
   "vec2 f=vec2(1920.,1080.),v=gl_FragCoord.xy/f.xy;"
   "float c=v.x-.5+.3*cos(s*1.6),e=v.y-.5+.4*sin(s*1.6);"
   "e*=.5625;"
   "float r=mod(sqrt(500.*(c*c+e*e))-s*8.,1.),u=v.x-.5+.3*cos(s*2.2-3.14),o=v.y-.5+.4*sin(s*2.2-3.14);"
   "o*=.5625;"
   "u+=sin(s/20.)*.2*sin(7.*v.y-s*2.);"
   "o+=sin(s/20.)*.2*sin(6.*v.x-s*3.);"
   "float y=step(0.,sin(sqrt(6000.*(u*u+o*o))));"
   "vec3 m=vec3(r*1.2,r,r*1.3)*(1.-y);"
   "float i=(1.-r)*y;"
   "vec3 t=vec3(i,i,i);"
   "t+=m;"
   "fragColor=vec4(t,1.);"
 "}";

#endif // EFFECT_H_
