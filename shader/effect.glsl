#version 450

out vec4 fragColor;
uniform float u_time = 0.;
float iTime = u_time;
//uniform float T = 0.;

void main()
{
    vec2 r = vec2(1920.0, 1080.0);
    vec2 uv = gl_FragCoord.xy/r.xy;

    float cx1 = uv.x-0.5+0.3*cos(iTime*1.6);
    float cy1 = uv.y-0.5+0.4*sin(iTime*1.6);
    cy1 *= 9./16.;
    
    float p1 = mod(sqrt(5e2*(cx1*cx1+cy1*cy1))-iTime*8.,1.);
    
    float cx2 = uv.x-0.5+0.3*cos(iTime*2.2-3.14);
    float cy2 = uv.y-0.5+0.4*sin(iTime*2.2-3.14);
    cy2 *= 9./16.;

    cx2 += sin(iTime/20.)*0.2*(sin(7.*uv.y-iTime*2.));	
    cy2 += sin(iTime/20.)*0.2*(sin(6.*uv.x-iTime*3.));

    float p2 = step(0., sin(sqrt(6e3*(cx2*cx2+cy2*cy2))));

    vec3 c1 = vec3(p1*1.2,p1,p1*1.3)*(1.-p2);
                   
    float c = ((1.-p1)*p2);
    
    vec3 col = vec3(c,c,c);
    col += c1;
    
    fragColor = vec4(col,1.0);
}
