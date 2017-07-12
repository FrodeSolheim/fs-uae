<?xml version="1.0" encoding="UTF-8"?>
<!--


-->
<shader language="GLSL">


    <vertex><![CDATA[
        uniform vec2 rubyTextureSize;

        void main()
        {
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                gl_TexCoord[0] = gl_MultiTexCoord0;
        }
    ]]></vertex>

/*
   CRT - Guest
   
   Copyright (C) 2017 guest(r) - guest.r@gmail.com

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
	
    <fragment outscale = "1.0"  filter="nearest"><![CDATA[
	
	uniform sampler2D rubyTexture;
	uniform vec2 rubyTextureSize;

	#define saturation 1.2    // 1.0 is normal saturation
	#define beam_min   1.2	  // dark area beam min - wide
	#define beam_max   2.6	  // bright area beam max - narrow
	
	const vec2 scale = vec2(0.2, 0.1);
	const vec3 dt = vec3(1.0,1.0,1.0);
	const vec3 Y  = vec3(0.299, 0.587, 0.114);

	vec4 yx = vec4(scale/rubyTextureSize,-scale/rubyTextureSize);

	vec3 texture2d (vec2 texcoord) {

		vec3 s00 = texture2D(rubyTexture, texcoord + yx.zw).xyz; s00*=s00;
		vec3 s20 = texture2D(rubyTexture, texcoord + yx.xw).xyz; s20*=s20;   
		vec3 s22 = texture2D(rubyTexture, texcoord + yx.xy).xyz; s22*=s22;
		vec3 s02 = texture2D(rubyTexture, texcoord + yx.zy).xyz; s02*=s02;

		float m1=dot(abs(s00-s22),dt)+0.001;
		float m2=dot(abs(s02-s20),dt)+0.001;

		return .5*(m2*(s00+s22)+m1*(s02+s20))/(m1+m2);
	}


float sw(float x, float l)
{
	float d = x;
	float bm = 10.0 + 4.0*l;
	float b = mix(beam_min,beam_max,l);
	d = exp2(-bm*pow(d,b));
	return d;
}

vec3 Mask1(float pos){

	float m = mod(int(pos),3);
	
	if (m == 0.0) return vec3(1.0, 0.0, 0.0); else
	if (m == 1.0) return vec3(0.0, 1.0, 0.0); else
	return vec3(0.0, 0.0, 1.0);
}    

vec3 Mask(float pos){

	float m = mod(int(pos),2);
	
	if (m == 0.0) return vec3(1.0, 0.0, 1.0); else
	return vec3(0.0, 1.0, 0.0); 
}  

void main()
{
	// Calculating texel coordinates

	vec2 size     = rubyTextureSize/scale;
	vec2 inv_size = scale/rubyTextureSize;
	
	vec2 OGL2Pos = gl_TexCoord[0].xy * size;
	vec2 fp = fract(OGL2Pos);
	vec2 dx = vec2(inv_size.x,0.0);
	vec2 dy = vec2(0.0, inv_size.y);
	vec2 g1 = vec2(inv_size.x,inv_size.y);
	vec2 g2 = vec2(-inv_size.x,inv_size.y);
	
	vec2 pC4 = floor(OGL2Pos) * inv_size + 0.5*inv_size;	
	
	// Reading the texels
	vec3 C0 = texture2d(pC4 - g1); 
	vec3 C1 = texture2d(pC4 - dy);
	vec3 C2 = texture2d(pC4 - g2);
	vec3 C3 = texture2d(pC4 - dx);
	vec3 C4 = texture2d(pC4     );
	vec3 C5 = texture2d(pC4 + dx);
	vec3 C6 = texture2d(pC4 + g2);
	vec3 C7 = texture2d(pC4 + dy);
	vec3 C8 = texture2d(pC4 + g1);
	
	vec3 ul, ur, dl, dr;
	float m1, m2;
	
	m1 = dot(abs(C0-C4),dt)+0.001;
	m2 = dot(abs(C1-C3),dt)+0.001;
	ul = (m2*(C0+C4)+m1*(C1+C3))/(m1+m2);  
	
	m1 = dot(abs(C1-C5),dt)+0.001;
	m2 = dot(abs(C2-C4),dt)+0.001;
	ur = (m2*(C1+C5)+m1*(C2+C4))/(m1+m2);
	
	m1 = dot(abs(C3-C7),dt)+0.001;
	m2 = dot(abs(C6-C4),dt)+0.001;
	dl = (m2*(C3+C7)+m1*(C6+C4))/(m1+m2);
	
	m1 = dot(abs(C4-C8),dt)+0.001;
	m2 = dot(abs(C5-C7),dt)+0.001;
	dr = (m2*(C4+C8)+m1*(C5+C7))/(m1+m2);
	
	vec3 color = 0.5*((dr*fp.x+dl*(1.0-fp.x))*fp.y+(ur*fp.x+ul*(1.0-fp.x))*(1.0-fp.y));

// applying bloom
	
	//float mx = max (color.r, max (color.g, color.b));
	//color = color*(1.0 + 0.10*pow(mx,0.5));
	color*=(1.7 - 0.4*length(color));
	color = min(color, 1.0);

// calculating scanlines
	
	float f = fract(gl_TexCoord[0].y * rubyTextureSize.y); f = f-0.5; float luma = length(color)/1.73205080757;
	color*=sw(abs(f),luma) + sw(f+1.0,luma) + sw(abs(f-1.0),luma);

// applying shadowmask

	color = pow(color, vec3(1.2));	
	color = color = mix(0.9*color, 1.1*color*Mask(gl_FragCoord.x), 0.5)*1.15;
	color = min(color, 1.0);
	
	color = pow(color, vec3(0.35));
	
	float l = length(color);
	color = normalize(pow(color, vec3(saturation)))*l;	
		
	gl_FragColor = vec4(color,1.0);
	}
    ]]></fragment>
	

</shader>
