#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;


void main( void ) {
vec2 p=(gl_FragCoord.xy-.5*resolution)/min(resolution.x,resolution.y);//
    vec3 c=vec3(0);
    float t0 = (time);
    for(int i=0;i<50;i++){
	float rad0 = 6.24*(float(i) + t0)/1.0;
        vec2 xy = vec2(
		0.3*cos(rad0 * 3.5),
		0.3*sin(rad0 * 3.0));
	    
	    
	float r = fract(0.3);
	float g = fract(float(i) / 100.4);
	float len = 0.0005/(length(p-xy));
	if(len<.005)
		len = 0.0;
	
    c += 8.0*len*vec3(r,g,1);
    }
	float alpha = 1.0;
	if(c.x<0.5)
		alpha = 0.0;
    gl_FragColor = vec4(c,alpha);
}