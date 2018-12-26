
varying lowp vec2 v_texCoord;
uniform  float time;
uniform  vec2  resolution;
void main()
{
 //   gl_FragColor = vec4(v_texCoord.x, v_texCoord.y, 0.0, 1.0);
	vec2 uv = (gl_FragCoord.xy/resolution.xy)-.5;
//	vec2 uv = v_texCoord.xy-.5;
	float iTime = time;
	float time = iTime * .1 + ((.25+.05*sin(iTime*.1))/(length(uv.xy)+.07))* 2.2;
	float si = sin(time);
	float co = cos(time);
	mat2 ma = mat2(co, si, -si, co);
  

	float c = 0.0;
	float v1 = 0.0;
	float v2 = 0.0;

	for (int i = 0; i < 50; i++)
	{
		float s = float(i) * .035;
		vec3 p = s * vec3(uv, 0.0);
		p.xy *= ma;
		p += vec3(.22,.3, s-1.5-sin(iTime*.13)*.1);
		for (int i = 0; i < 8; i++)
		{
			p = abs(p) / dot(p,p) - 0.659;
		}
		v1 += dot(p,p)*.0015 * (1.8+sin(length(uv.xy*13.0)+.5-time*.2));
		v2 += dot(p,p)*.0015 * (1.5+sin(length(uv.xy*13.5)+2.2-time*.3));
		c = length(p.xy*.5) * .35;
	}
	float len = length(uv);
	
		
	v1 *= smoothstep(.7, .0, len);
	v2 *= smoothstep(.6, .0, len);

	float re = clamp(c, 0.0, 1.0);
	float gr = clamp((v1+c)*.25, 0.0, 1.0);
	float bl = clamp(v2, 0.0, 1.0);
	vec3 col = vec3(re, gr, bl) + smoothstep(0.15, .0, len) * .9;
	float alpha = col.x + col.y+col.z;
//	if(abs(gl_FragCoord.x - resolution.x/2) + abs(gl_FragCoord.y - resolution.y/2))>resolution.x/2)
//		alpha = 0;
	
//	if(gl_FragCoord.x < 100)
//		col = vec3(gl_FragCoord.x/255.0,gl_FragCoord.x/255.0,gl_FragCoord.x/255.0);
//		col = vec3(1.0,1.0,1.0);
	if(alpha<0.5)
		alpha = 0;

	gl_FragColor=vec4(col, alpha);    
	
}
/*
#ifdef GL_ES 
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

//
//  Trip Me Baby One More Time
//
// Meditative tweak by psyreco
//
uniform float giTime;
uniform vec2 mouse;
uniform vec2 resolution;

mat2 rotate(float a) 
{
	float c = cos(a),
	s = sin(a);
	return mat2(c, -s, s, c);
}


void main() {
	vec2 uv = 1.9*(2. * gl_FragCoord.xy - resolution) / resolution.y;
	vec3 color = vec3(0);
	uv.y+=0.6;
	if (uv.y < 0.0)   // horizontal mirroring
	{ 
	  uv.y *= 6.0;
	  color = vec3(-0.3);  // darker
	}
	vec3 rd = vec3(uv, (sin(giTime*.02+sin(giTime*0.06)*2.))*uv.y);
		
	float s = 0.5, t = .5;
	for (int i = 0; i < 9; i++) {
		//t = sin(t+s);
		rd = abs(abs(abs(rd) / abs(dot(sin(rd)+rd,sin(rd)+rd)))); // kali iteration!! Thanks Kali
		rd -= s * t;
		rd.xy *= rotate(0.1 + giTime *0.017);
		rd.xz *= rotate(0.2 - giTime *0.053);
		rd.zy *= rotate(0.3 + giTime *0.081);
		//rd -= s;
		s *= 0.6;
		t = -sin(t-s)*1.73;
		float b = 0.005;
		float ax = abs(rd.x);
		float ay = abs(rd.y);
		float az = abs(rd.z);
		color.gb -= .003 / max(abs(rd.x*0.2), abs(rd.y*0.8));
		color.gb +=  length(vec2(ax, ay))*0.08/max(ax*0.2, 6.2*ay);
		color.rb -= .002 / max(abs(rd.y*0.6), abs(rd.z*0.6));
		color.rb +=  length(vec2(ay, az))*0.09/max(ay*0.2, 6.2*az);
		color.rg += .001 / max(abs(rd.x*0.7), abs(rd.z*0.9));
		color.rg +=  length(vec2(ax, az))*0.08/max(ax*0.2, 6.2*az);
	}
	color *= 0.5;
	color = vec3(0.1,0.1,0.1);
	gl_FragColor = vec4(color, 0.1);
}

*/