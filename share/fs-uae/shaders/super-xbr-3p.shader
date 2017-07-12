<?xml version="1.0" encoding="UTF-8"?>
<!--

/*	
  
  *******  Super XBR Shader - pass0  *******
   
  Copyright (c) 2015 Hyllian - sergiogdb@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

*/ 

-->
<shader language="GLSL">

    <vertex><![CDATA[
        uniform vec2 rubyTextureSize;

        void main()
        {
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                gl_TexCoord[0] = gl_MultiTexCoord0;
				
				float dx = 1.0/rubyTextureSize.x;
				float dy = 1.0/rubyTextureSize.y;
				
				gl_TexCoord[1] = gl_TexCoord[0].xyxy + vec4(-dx, -dy, 2.0*dx, 2.0*dy);
				gl_TexCoord[2] = gl_TexCoord[0].xyxy + vec4(  0, -dy,     dx, 2.0*dy);
				gl_TexCoord[3] = gl_TexCoord[0].xyxy + vec4(-dx,   0, 2.0*dx,     dy);
				gl_TexCoord[4] = gl_TexCoord[0].xyxy + vec4(  0,   0,     dx,     dy); 
        }
    ]]></vertex>
	
    <fragment scale="1.0" filter="nearest"><![CDATA[
	
	uniform sampler2D rubyTexture;
	uniform vec2 rubyTextureSize;

	#define XBR_EDGE_STR  0.6
	#define XBR_WEIGHT  1.0
	#define XBR_ANTI_RINGING  1.0
	
#define mul(a,b) (b*a)

#define wp1  1.0
#define wp2  0.0
#define wp3  0.0
#define wp4  2.0
#define wp5 -1.0
#define wp6  0.0

#define weight1 (XBR_WEIGHT*1.29633/10.0)
#define weight2 (XBR_WEIGHT*1.75068/10.0/2.0)

const vec3 Y = vec3(.2126, .7152, .0722);

float RGBtoYUV(vec3 color)
{
  return dot(color, Y);
}

float df(float A, float B)
{
  return abs(A-B);
}

/*
                              P1
     |P0|B |C |P1|         C     F4          |a0|b1|c2|d3|
     |D |E |F |F4|      B     F     I4       |b0|c1|d2|e3|   |e1|i1|i2|e2|
     |G |H |I |I4|   P0    E  A  I     P3    |c0|d1|e2|f3|   |e3|i3|i4|e4|
     |P2|H5|I5|P3|      D     H     I5       |d0|e1|f2|g3|
                           G     H5
                              P2
*/


float d_wd(float b0, float b1, float c0, float c1, float c2, float d0, float d1, float d2, float d3, float e1, float e2, float e3, float f2, float f3)
{
	return (wp1*(df(c1,c2) + df(c1,c0) + df(e2,e1) + df(e2,e3)) + wp2*(df(d2,d3) + df(d0,d1)) + wp3*(df(d1,d3) + df(d0,d2)) + wp4*df(d1,d2) + wp5*(df(c0,c2) + df(e1,e3)) + wp6*(df(b0,b1) + df(f2,f3)));
}

float hv_wd(float i1, float i2, float i3, float i4, float e1, float e2, float e3, float e4)
{
	return ( wp4*(df(i1,i2)+df(i3,i4)) + wp1*(df(i1,e1)+df(i2,e2)+df(i3,e3)+df(i4,e4)) + wp3*(df(i1,e2)+df(i3,e4)+df(e1,i2)+df(e3,i4)));
}

vec3 min4(vec3 a, vec3 b, vec3 c, vec3 d)
{
    return min(a, min(b, min(c, d)));
}
vec3 max4(vec3 a, vec3 b, vec3 c, vec3 d)
{
    return max(a, max(b, max(c, d)));
}
 	
	
	void main()
	{
	vec3 P0 = texture2D(rubyTexture, gl_TexCoord[1].xy).xyz;
	vec3 P1 = texture2D(rubyTexture, gl_TexCoord[1].zy).xyz;
	vec3 P2 = texture2D(rubyTexture, gl_TexCoord[1].xw).xyz;
	vec3 P3 = texture2D(rubyTexture, gl_TexCoord[1].zw).xyz;

	vec3  B = texture2D(rubyTexture, gl_TexCoord[2].xy).xyz;
	vec3  C = texture2D(rubyTexture, gl_TexCoord[2].zy).xyz;
	vec3 H5 = texture2D(rubyTexture, gl_TexCoord[2].xw).xyz;
	vec3 I5 = texture2D(rubyTexture, gl_TexCoord[2].zw).xyz;

	vec3  D = texture2D(rubyTexture, gl_TexCoord[3].xy).xyz;
	vec3 F4 = texture2D(rubyTexture, gl_TexCoord[3].zy).xyz;
	vec3  G = texture2D(rubyTexture, gl_TexCoord[3].xw).xyz;
	vec3 I4 = texture2D(rubyTexture, gl_TexCoord[3].zw).xyz;

	vec3  E = texture2D(rubyTexture, gl_TexCoord[4].xy).xyz;
	vec3  F = texture2D(rubyTexture, gl_TexCoord[4].zy).xyz;
	vec3  H = texture2D(rubyTexture, gl_TexCoord[4].xw).xyz;
	vec3  I = texture2D(rubyTexture, gl_TexCoord[4].zw).xyz;

	float b = RGBtoYUV( B );
	float c = RGBtoYUV( C );
	float d = RGBtoYUV( D );
	float e = RGBtoYUV( E );
	float f = RGBtoYUV( F );
	float g = RGBtoYUV( G );
	float h = RGBtoYUV( H );
	float i = RGBtoYUV( I );

	float i4 = RGBtoYUV( I4 ); float p0 = RGBtoYUV( P0 );
	float i5 = RGBtoYUV( I5 ); float p1 = RGBtoYUV( P1 );
	float h5 = RGBtoYUV( H5 ); float p2 = RGBtoYUV( P2 );
	float f4 = RGBtoYUV( F4 ); float p3 = RGBtoYUV( P3 );


	/* Calc edgeness in diagonal directions. */
	float d_edge  = (d_wd( d, b, g, e, c, p2, h, f, p1, h5, i, f4, i5, i4 ) - d_wd( c, f4, b, f, i4, p0, e, i, p3, d, h, i5, g, h5 ));

	/* Calc edgeness in horizontal/vertical directions. */
	float hv_edge = (hv_wd(f, i, e, h, c, i5, b, h5) - hv_wd(e, f, h, i, d, f4, g, i4));

	float limits = XBR_EDGE_STR + 0.000001;
	float edge_strength = smoothstep(0.0, limits, abs(d_edge));

	/* Filter weights. Two taps only. */
	vec4 w1 = vec4(-weight1, weight1+0.5, weight1+0.5, -weight1);
	vec4 w2 = vec4(-weight2, weight2+0.25, weight2+0.25, -weight2);

	/* Filtering and normalization in four direction generating four colors. */
    vec3 c1 = mul(w1, mat4x3( P2,   H,   F,   P1 ));
    vec3 c2 = mul(w1, mat4x3( P0,   E,   I,   P3 ));
	vec3 c3 = mul(w2, mat4x3(D+G, E+H, F+I, F4+I4));
    vec3 c4 = mul(w2, mat4x3(C+B, F+E, I+H, I5+H5));

	/* Smoothly blends the two strongest directions (one in diagonal and the other in vert/horiz direction). */
	vec3 color =  mix(mix(c1, c2, step(0.0, d_edge)), mix(c3, c4, step(0.0, hv_edge)), 1. - edge_strength);

	/* Anti-ringing code. */
	vec3 min_sample = min4( E, F, H, I ) + (1.-XBR_ANTI_RINGING)*mix((P2-H)*(F-P1), (P0-E)*(I-P3), step(0.0, d_edge));
	vec3 max_sample = max4( E, F, H, I ) - (1.-XBR_ANTI_RINGING)*mix((P2-H)*(F-P1), (P0-E)*(I-P3), step(0.0, d_edge));
	color = clamp(color, min_sample, max_sample); 
	
	gl_FragColor.xyz = color;
	gl_FragColor.a = 1.0;
	}
    ]]></fragment>
	

/*
   
  *******  Super XBR Shader - pass1  *******
   
  Copyright (c) 2015 Hyllian - sergiogdb@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

*/ 	
	
    <vertex><![CDATA[
        uniform vec2 rubyTextureSize;

        void main()
        {
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                gl_TexCoord[0] = gl_MultiTexCoord0;
        }
    ]]></vertex>
	
    <fragment scale="2.0" filter="nearest"><![CDATA[
	
	uniform sampler2D rubyTexture;
	uniform sampler2D rubyOrigTexture;	
	uniform vec2 rubyTextureSize;

const float XBR_EDGE_STR = 0.0;
const float XBR_WEIGHT = 0.0;
const float XBR_ANTI_RINGING = 0.0;

#define mul(a,b) (b*a)

#define wp1  1.0
#define wp2  0.0
#define wp3  0.0
#define wp4  4.0
#define wp5  0.0
#define wp6  0.0

#define weight1 (XBR_WEIGHT*1.75068/10.0)
#define weight2 (XBR_WEIGHT*1.29633/10.0/2.0)

const vec3 Y = vec3(.2126, .7152, .0722);
 
float RGBtoYUV(vec3 color)
{
  return dot(color, Y);
}

float df(float A, float B)
{
  return abs(A-B);
}

/*
                              P1
     |P0|B |C |P1|         C     F4          |a0|b1|c2|d3|
     |D |E |F |F4|      B     F     I4       |b0|c1|d2|e3|   |e1|i1|i2|e2|
     |G |H |I |I4|   P0    E  A  I     P3    |c0|d1|e2|f3|   |e3|i3|i4|e4|
     |P2|H5|I5|P3|      D     H     I5       |d0|e1|f2|g3|
                           G     H5
                              P2
*/

float d_wd(float b0, float b1, float c0, float c1, float c2, float d0, float d1, float d2, float d3, float e1, float e2, float e3, float f2, float f3)
{
	return (wp1*(df(c1,c2) + df(c1,c0) + df(e2,e1) + df(e2,e3)) + wp2*(df(d2,d3) + df(d0,d1)) + wp3*(df(d1,d3) + df(d0,d2)) + wp4*df(d1,d2) + wp5*(df(c0,c2) + df(e1,e3)) + wp6*(df(b0,b1) + df(f2,f3)));
}

float hv_wd(float i1, float i2, float i3, float i4, float e1, float e2, float e3, float e4)
{
	return ( wp4*(df(i1,i2)+df(i3,i4)) + wp1*(df(i1,e1)+df(i2,e2)+df(i3,e3)+df(i4,e4)) + wp3*(df(i1,e2)+df(i3,e4)+df(e1,i2)+df(e3,i4)));
}

vec3 min4(vec3 a, vec3 b, vec3 c, vec3 d)
{
    return min(a, min(b, min(c, d)));
}
vec3 max4(vec3 a, vec3 b, vec3 c, vec3 d)
{
    return max(a, max(b, max(c, d)));
} 

 	
	void main()
	{
	//Skip pixels on wrong grid
	vec2 fp = fract(gl_TexCoord[0].xy*rubyTextureSize);
	vec2 dir = fp - vec2(0.5,0.5);
 	if ((dir.x*dir.y)>0.0){
	gl_FragColor = (fp.x>0.5) ? texture2D(rubyTexture, gl_TexCoord[0].xy) : texture2D(rubyOrigTexture, gl_TexCoord[0].xy);
	}else{

	vec2 g1 = (fp.x>0.5) ? vec2(0.5/rubyTextureSize.x, 0.0) : vec2(0.0, 0.5/rubyTextureSize.y);
	vec2 g2 = (fp.x>0.5) ? vec2(0.0, 0.5/rubyTextureSize.y) : vec2(0.5/rubyTextureSize.x, 0.0);

	vec3 P0 = texture2D(rubyOrigTexture,	gl_TexCoord[0].xy -3.0*g1        ).xyz;
	vec3 P1 = texture2D(rubyTexture,	gl_TexCoord[0].xy         -3.0*g2).xyz;
	vec3 P2 = texture2D(rubyTexture,	gl_TexCoord[0].xy         +3.0*g2).xyz;
	vec3 P3 = texture2D(rubyOrigTexture,	gl_TexCoord[0].xy +3.0*g1        ).xyz;

	vec3  B = texture2D(rubyTexture,	gl_TexCoord[0].xy -2.0*g1     -g2).xyz;
	vec3  C = texture2D(rubyOrigTexture,	gl_TexCoord[0].xy     -g1 -2.0*g2).xyz;
	vec3  D = texture2D(rubyTexture,	gl_TexCoord[0].xy -2.0*g1     +g2).xyz;
	vec3  E = texture2D(rubyOrigTexture,	gl_TexCoord[0].xy     -g1        ).xyz;
	vec3  F = texture2D(rubyTexture,	gl_TexCoord[0].xy             -g2).xyz;
	vec3  G = texture2D(rubyOrigTexture,	gl_TexCoord[0].xy     -g1 +2.0*g2).xyz;
	vec3  H = texture2D(rubyTexture,	gl_TexCoord[0].xy             +g2).xyz;
	vec3  I = texture2D(rubyOrigTexture,	gl_TexCoord[0].xy     +g1        ).xyz;

	vec3 F4 = texture2D(rubyOrigTexture,	gl_TexCoord[0].xy     +g1 -2.0*g2).xyz;
	vec3 I4 = texture2D(rubyTexture,	gl_TexCoord[0].xy +2.0*g1     -g2).xyz;
	vec3 H5 = texture2D(rubyOrigTexture,	gl_TexCoord[0].xy     +g1 +2.0*g2).xyz;
	vec3 I5 = texture2D(rubyTexture,	gl_TexCoord[0].xy +2.0*g1     +g2).xyz;

	float b = RGBtoYUV( B );
	float c = RGBtoYUV( C );
	float d = RGBtoYUV( D );
	float e = RGBtoYUV( E );
	float f = RGBtoYUV( F );
	float g = RGBtoYUV( G );
	float h = RGBtoYUV( H );
	float i = RGBtoYUV( I );

	float i4 = RGBtoYUV( I4 ); float p0 = RGBtoYUV( P0 );
	float i5 = RGBtoYUV( I5 ); float p1 = RGBtoYUV( P1 );
	float h5 = RGBtoYUV( H5 ); float p2 = RGBtoYUV( P2 );
	float f4 = RGBtoYUV( F4 ); float p3 = RGBtoYUV( P3 );

	/* Calc edgeness in diagonal directions. */
	float d_edge  = (d_wd( d, b, g, e, c, p2, h, f, p1, h5, i, f4, i5, i4 ) - d_wd( c, f4, b, f, i4, p0, e, i, p3, d, h, i5, g, h5 ));

	/* Calc edgeness in horizontal/vertical directions. */
	float hv_edge = (hv_wd(f, i, e, h, c, i5, b, h5) - hv_wd(e, f, h, i, d, f4, g, i4));

	float limits = XBR_EDGE_STR + 0.000001;
	float edge_strength = smoothstep(0.0, limits, abs(d_edge));

	/* Filter weights. Two taps only. */
	vec4 w1 = vec4(-weight1, weight1+0.5, weight1+0.5, -weight1);
	vec4 w2 = vec4(-weight2, weight2+0.25, weight2+0.25, -weight2);

	/* Filtering and normalization in four direction generating four colors. */
    vec3 c1 = mul(w1, mat4x3( P2,   H,   F,   P1 ));
    vec3 c2 = mul(w1, mat4x3( P0,   E,   I,   P3 ));
	vec3 c3 = mul(w2, mat4x3(D+G, E+H, F+I, F4+I4));
    vec3 c4 = mul(w2, mat4x3(C+B, F+E, I+H, I5+H5));

	/* Smoothly blends the two strongest directions (one in diagonal and the other in vert/horiz direction). */
	vec3 color =  mix(mix(c1, c2, step(0.0, d_edge)), mix(c3, c4, step(0.0, hv_edge)), 1. - edge_strength);

	/* Anti-ringing code. */
	vec3 min_sample = min4( E, F, H, I ) + (1.-XBR_ANTI_RINGING)*mix((P2-H)*(F-P1), (P0-E)*(I-P3), step(0.0, d_edge));
	vec3 max_sample = max4( E, F, H, I ) - (1.-XBR_ANTI_RINGING)*mix((P2-H)*(F-P1), (P0-E)*(I-P3), step(0.0, d_edge));
	color = clamp(color, min_sample, max_sample); 

	gl_FragColor.xyz = color;
	gl_FragColor.a = 1.0;
	}
	}
    ]]></fragment>	
	


	
/*
   
  *******  Super XBR Shader - pass2  *******  This pass is optional.
   
  Copyright (c) 2015 Hyllian - sergiogdb@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

*/ 	

    <vertex><![CDATA[
        uniform vec2 rubyTextureSize;

        void main()
        {
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                gl_TexCoord[0] = gl_MultiTexCoord0;
				
				float dx = 1.0/rubyTextureSize.x;
				float dy = 1.0/rubyTextureSize.y;
				
				gl_TexCoord[1] = gl_TexCoord[0].xyxy + vec4(-dx, -dy, 2.0*dx, 2.0*dy);
				gl_TexCoord[2] = gl_TexCoord[0].xyxy + vec4(  0, -dy,     dx, 2.0*dy);
				gl_TexCoord[3] = gl_TexCoord[0].xyxy + vec4(-dx,   0, 2.0*dx,     dy);
				gl_TexCoord[4] = gl_TexCoord[0].xyxy + vec4(  0,   0,     dx,     dy); 
        }
    ]]></vertex>
	
    <fragment scale="1.0" filter="nearest"><![CDATA[
	
	uniform sampler2D rubyTexture;
	uniform vec2 rubyTextureSize;

const float XBR_EDGE_STR = 0.7;
const float XBR_WEIGHT = 0.6;
const float XBR_ANTI_RINGING = 0.0;
	
#define mul(a,b) (b*a)

#define wp1  1.0
#define wp2  0.0
#define wp3  0.0
#define wp4  0.0
#define wp5 -1.0
#define wp6  0.0

#define weight1 (XBR_WEIGHT*1.29633/10.0)
#define weight2 (XBR_WEIGHT*1.75068/10.0/2.0)

const vec3 Y = vec3(.2126, .7152, .0722);

float RGBtoYUV(vec3 color)
{
  return dot(color, Y);
}

float df(float A, float B)
{
  return abs(A-B);
}

/*
                              P1
     |P0|B |C |P1|         C     F4          |a0|b1|c2|d3|
     |D |E |F |F4|      B     F     I4       |b0|c1|d2|e3|   |e1|i1|i2|e2|
     |G |H |I |I4|   P0    E  A  I     P3    |c0|d1|e2|f3|   |e3|i3|i4|e4|
     |P2|H5|I5|P3|      D     H     I5       |d0|e1|f2|g3|
                           G     H5
                              P2
*/

float d_wd(float b0, float b1, float c0, float c1, float c2, float d0, float d1, float d2, float d3, float e1, float e2, float e3, float f2, float f3)
{
	return (wp1*(df(c1,c2) + df(c1,c0) + df(e2,e1) + df(e2,e3)) + wp2*(df(d2,d3) + df(d0,d1)) + wp3*(df(d1,d3) + df(d0,d2)) + wp4*df(d1,d2) + wp5*(df(c0,c2) + df(e1,e3)) + wp6*(df(b0,b1) + df(f2,f3)));
}

float hv_wd(float i1, float i2, float i3, float i4, float e1, float e2, float e3, float e4)
{
	return ( wp4*(df(i1,i2)+df(i3,i4)) + wp1*(df(i1,e1)+df(i2,e2)+df(i3,e3)+df(i4,e4)) + wp3*(df(i1,e2)+df(i3,e4)+df(e1,i2)+df(e3,i4)));
}

vec3 min4(vec3 a, vec3 b, vec3 c, vec3 d)
{
    return min(a, min(b, min(c, d)));
}
vec3 max4(vec3 a, vec3 b, vec3 c, vec3 d)
{
    return max(a, max(b, max(c, d)));
} 
	
	void main()
	{
	vec3 P0 = texture2D(rubyTexture, gl_TexCoord[1].xy).xyz;
	vec3 P1 = texture2D(rubyTexture, gl_TexCoord[1].zy).xyz;
	vec3 P2 = texture2D(rubyTexture, gl_TexCoord[1].xw).xyz;
	vec3 P3 = texture2D(rubyTexture, gl_TexCoord[1].zw).xyz;

	vec3  B = texture2D(rubyTexture, gl_TexCoord[2].xy).xyz;
	vec3  C = texture2D(rubyTexture, gl_TexCoord[2].zy).xyz;
	vec3 H5 = texture2D(rubyTexture, gl_TexCoord[2].xw).xyz;
	vec3 I5 = texture2D(rubyTexture, gl_TexCoord[2].zw).xyz;

	vec3  D = texture2D(rubyTexture, gl_TexCoord[3].xy).xyz;
	vec3 F4 = texture2D(rubyTexture, gl_TexCoord[3].zy).xyz;
	vec3  G = texture2D(rubyTexture, gl_TexCoord[3].xw).xyz;
	vec3 I4 = texture2D(rubyTexture, gl_TexCoord[3].zw).xyz;

	vec3  E = texture2D(rubyTexture, gl_TexCoord[4].xy).xyz;
	vec3  F = texture2D(rubyTexture, gl_TexCoord[4].zy).xyz;
	vec3  H = texture2D(rubyTexture, gl_TexCoord[4].xw).xyz;
	vec3  I = texture2D(rubyTexture, gl_TexCoord[4].zw).xyz;

	float b = RGBtoYUV( B );
	float c = RGBtoYUV( C );
	float d = RGBtoYUV( D );
	float e = RGBtoYUV( E );
	float f = RGBtoYUV( F );
	float g = RGBtoYUV( G );
	float h = RGBtoYUV( H );
	float i = RGBtoYUV( I );

	float i4 = RGBtoYUV( I4 ); float p0 = RGBtoYUV( P0 );
	float i5 = RGBtoYUV( I5 ); float p1 = RGBtoYUV( P1 );
	float h5 = RGBtoYUV( H5 ); float p2 = RGBtoYUV( P2 );
	float f4 = RGBtoYUV( F4 ); float p3 = RGBtoYUV( P3 );

/*
                              P1
     |P0|B |C |P1|         C     F4          |a0|b1|c2|d3|
     |D |E |F |F4|      B     F     I4       |b0|c1|d2|e3|   |e1|i1|i2|e2|
     |G |H |I |I4|   P0    E  A  I     P3    |c0|d1|e2|f3|   |e3|i3|i4|e4|
     |P2|H5|I5|P3|      D     H     I5       |d0|e1|f2|g3|
                           G     H5
                              P2
*/

	/* Calc edgeness in diagonal directions. */
	float d_edge  = (d_wd( d, b, g, e, c, p2, h, f, p1, h5, i, f4, i5, i4 ) - d_wd( c, f4, b, f, i4, p0, e, i, p3, d, h, i5, g, h5 ));

	/* Calc edgeness in horizontal/vertical directions. */
	float hv_edge = (hv_wd(f, i, e, h, c, i5, b, h5) - hv_wd(e, f, h, i, d, f4, g, i4));

	float limits = XBR_EDGE_STR + 0.000001;
	float edge_strength = smoothstep(0.0, limits, abs(d_edge));

	/* Filter weights. Two taps only. */
	vec4 w1 = vec4(-weight1, weight1+0.5, weight1+0.5, -weight1);
	vec4 w2 = vec4(-weight2, weight2+0.25, weight2+0.25, -weight2);

	/* Filtering and normalization in four direction generating four colors. */
	vec3 c1 = mul(w1, mat4x3( P2,   H,   F,   P1 ));
	vec3 c2 = mul(w1, mat4x3( P0,   E,   I,   P3 ));
	vec3 c3 = mul(w2, mat4x3(D+G, E+H, F+I, F4+I4));
	vec3 c4 = mul(w2, mat4x3(C+B, F+E, I+H, I5+H5));

	/* Smoothly blends the two strongest directions (one in diagonal and the other in vert/horiz direction). */
	vec3 color =  mix(mix(c1, c2, step(0.0, d_edge)), mix(c3, c4, step(0.0, hv_edge)), 1. - edge_strength);

	/* Anti-ringing code. */
	vec3 min_sample = min4( E, F, H, I ) + (1.-XBR_ANTI_RINGING)*mix((P2-H)*(F-P1), (P0-E)*(I-P3), step(0.0, d_edge));
	vec3 max_sample = max4( E, F, H, I ) - (1.-XBR_ANTI_RINGING)*mix((P2-H)*(F-P1), (P0-E)*(I-P3), step(0.0, d_edge));
	color = clamp(color, min_sample, max_sample); 
	
	gl_FragColor.xyz = color;
	gl_FragColor.a = 1.0;
	}
    ]]></fragment>


	
	
    <vertex><![CDATA[
        uniform vec2 rubyTextureSize;

        void main()
        {
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                gl_TexCoord[0] = gl_MultiTexCoord0;
        }
    ]]></vertex>

/*
   Hyllian's jinc windowed-jinc 2-lobe sharper with anti-ringing Shader
   
   Copyright (C) 2011-2014 Hyllian/Jararaca - sergiogdb@gmail.com

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
	
    <fragment outscale="1.0" filter="nearest"><![CDATA[
	
	uniform sampler2D rubyTexture;
	uniform vec2 rubyTextureSize;

	const   float halfpi            = 1.5707963267948966192313216916398;
	const   float pi                = 3.1415926535897932384626433832795;
	const   float JINC2_WINDOW_SINC = 0.42;
	const   float JINC2_SINC        = 0.92;
	const   float wa                = JINC2_WINDOW_SINC*pi;
	const   float wb                = JINC2_SINC*pi;
	const   float JINC2_AR_STRENGTH = 0.0;

	// Calculates the distance between two points
	float d(vec2 pt1, vec2 pt2)
	{
	vec2 v = pt2 - pt1;
	return sqrt(dot(v,v));
	}

	vec3 min4(vec3 a, vec3 b, vec3 c, vec3 d)
	{
		return min(a, min(b, min(c, d)));
	}

	vec3 max4(vec3 a, vec3 b, vec3 c, vec3 d)
	{
		return max(a, max(b, max(c, d)));
	}

	vec4 resampler(vec4 x)
	{
		vec4 res;
		res = (x==vec4(0.0, 0.0, 0.0, 0.0)) ?  vec4(wa*wb)  :  sin(x*wa)*sin(x*wb)/(x*x);
		return res;
	}


void main()
{
	vec2 OGLSize  = rubyTextureSize;

    vec3 color;
    vec4 weights[4];

    vec2 Dx = vec2(1.0, 0.0);
    vec2 Dy = vec2(0.0, 1.0);

    vec2 pc = (gl_TexCoord[0].xy + vec2(0.0000001,0.0000001))*OGLSize;

    vec2 tc = (floor(pc-vec2(0.5,0.5))+vec2(0.5,0.5));
     
    weights[0] = resampler(vec4(d(pc, tc    -Dx    -Dy), d(pc, tc           -Dy), d(pc, tc    +Dx    -Dy), d(pc, tc+2.0*Dx    -Dy)));
    weights[1] = resampler(vec4(d(pc, tc    -Dx       ), d(pc, tc              ), d(pc, tc    +Dx       ), d(pc, tc+2.0*Dx       )));
    weights[2] = resampler(vec4(d(pc, tc    -Dx    +Dy), d(pc, tc           +Dy), d(pc, tc    +Dx    +Dy), d(pc, tc+2.0*Dx    +Dy)));
    weights[3] = resampler(vec4(d(pc, tc    -Dx+2.0*Dy), d(pc, tc       +2.0*Dy), d(pc, tc    +Dx+2.0*Dy), d(pc, tc+2.0*Dx+2.0*Dy)));

	vec2 dx = Dx/OGLSize;
	vec2 dy = Dy/OGLSize;
	tc = tc/OGLSize; 
	
    vec3 c00 = texture2D(rubyTexture, tc    -dx    -dy).xyz;
    vec3 c10 = texture2D(rubyTexture, tc           -dy).xyz;
    vec3 c20 = texture2D(rubyTexture, tc    +dx    -dy).xyz;
    vec3 c30 = texture2D(rubyTexture, tc+2.0*dx    -dy).xyz;
    vec3 c01 = texture2D(rubyTexture, tc    -dx       ).xyz;
    vec3 c11 = texture2D(rubyTexture, tc              ).xyz;
    vec3 c21 = texture2D(rubyTexture, tc    +dx       ).xyz;
    vec3 c31 = texture2D(rubyTexture, tc+2.0*dx       ).xyz;
    vec3 c02 = texture2D(rubyTexture, tc    -dx    +dy).xyz;
    vec3 c12 = texture2D(rubyTexture, tc           +dy).xyz;
    vec3 c22 = texture2D(rubyTexture, tc    +dx    +dy).xyz;
    vec3 c32 = texture2D(rubyTexture, tc+2.0*dx    +dy).xyz;
    vec3 c03 = texture2D(rubyTexture, tc    -dx+2.0*dy).xyz;
    vec3 c13 = texture2D(rubyTexture, tc       +2.0*dy).xyz;
    vec3 c23 = texture2D(rubyTexture, tc    +dx+2.0*dy).xyz;
    vec3 c33 = texture2D(rubyTexture, tc+2.0*dx+2.0*dy).xyz;

    color = vec3(dot(weights[0], vec4(c00.x, c10.x, c20.x, c30.x)), dot(weights[0], vec4(c00.y, c10.y, c20.y, c30.y)), dot(weights[0], vec4(c00.z, c10.z, c20.z, c30.z)));
    color+= vec3(dot(weights[1], vec4(c01.x, c11.x, c21.x, c31.x)), dot(weights[1], vec4(c01.y, c11.y, c21.y, c31.y)), dot(weights[1], vec4(c01.z, c11.z, c21.z, c31.z)));
    color+= vec3(dot(weights[2], vec4(c02.x, c12.x, c22.x, c32.x)), dot(weights[2], vec4(c02.y, c12.y, c22.y, c32.y)), dot(weights[2], vec4(c02.z, c12.z, c22.z, c32.z)));
    color+= vec3(dot(weights[3], vec4(c03.x, c13.x, c23.x, c33.x)), dot(weights[3], vec4(c03.y, c13.y, c23.y, c33.y)), dot(weights[3], vec4(c03.z, c13.z, c23.z, c33.z)));
    color = color/(dot(weights[0], vec4(1.0,1.0,1.0,1.0)) + dot(weights[1], vec4(1.0,1.0,1.0,1.0)) + dot(weights[2], vec4(1.0,1.0,1.0,1.0)) + dot(weights[3], vec4(1.0,1.0,1.0,1.0)));

	// Anti-ringing
	//  Get min/max samples
	pc = gl_TexCoord[0].xy;
	c00 = texture2D(rubyTexture, pc              ).xyz;
	c11 = texture2D(rubyTexture, pc    +dx       ).xyz;
	c21 = texture2D(rubyTexture, pc    -dx       ).xyz;
	c12 = texture2D(rubyTexture, pc           +dy).xyz;
	c22 = texture2D(rubyTexture, pc           -dy).xyz;

	//  Get min/max samples
    vec3 min_sample = min4(c11, c21, c12, c22);
    vec3 max_sample = max4(c11, c21, c12, c22);
	
	vec3 aux = color;

	color = clamp(color, min_sample, max_sample);
	color = mix(aux, color, JINC2_AR_STRENGTH); 
 
    // final sum and weight normalization
    gl_FragColor.xyz = color;
	gl_FragColor.a = 1.0;
	}
    ]]></fragment>
	
</shader>
