<?xml version="1.0" encoding="UTF-8"?>
<shader language="GLSL">

/*
	ScaleFX - 4 Passes
	by Sp00kyFox, 2016-03-30

ScaleFX is an edge interpolation algorithm specialized in pixel art. It was
originally intended as an improvement upon Scale3x but became a new filter in
its own right.
ScaleFX interpolates edges up to level 6 and makes smooth transitions between
different slopes. The filtered picture will only consist of colours present
in the original.

Pass 0 prepares metric data for the next pass.
Pass 1 resolves ambiguous configurations of corner candidates at pixel junctions.
Pass 2 determines which edge level is present and prepares tags for subpixel output in the final pass.
Pass 3 outputs subpixels based on previously calculated tags.

Copyright (c) 2016 Sp00kyFox - ScaleFX@web.de

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

ported to GLSL by guest.r
*/

// ScaleFX pass 0
<vertex><![CDATA[
uniform vec2 rubyTextureSize;

void main(void) {
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec2 ps = 1.0/rubyTextureSize;
	float dx = ps.x, dy = ps.y;

	gl_TexCoord[1] = gl_TexCoord[0].xxxy + vec4(  -dx,   0.0, dx,  -dy);	// A, B, C
	gl_TexCoord[2] = gl_TexCoord[0].xxxy + vec4(  -dx,   0.0, dx,  0.0);	// D, E, F
}
]]></vertex>

<fragment scale="1.0" filter="nearest"><![CDATA[
	uniform sampler2D rubyTexture;

	float eq(vec3 A, vec3 B)
	{
		float  r = 0.5 * (A.r + B.r);
		vec3 d = A - B;
		vec3 c = vec3(2.0 + r, 4.0, 3.0 - r);

		return 1.0 - sqrt(dot(c*d, d)) / 3.0;
	}
void main()
{
	/*	grid		metric

		A B C		x y z
		  E F		  o w
	*/
	
	// read texels
	vec3 A = texture2D(rubyTexture, gl_TexCoord[1].xw).rgb;
	vec3 B = texture2D(rubyTexture, gl_TexCoord[1].yw).rgb;
	vec3 C = texture2D(rubyTexture, gl_TexCoord[1].zw).rgb;
	vec3 E = texture2D(rubyTexture, gl_TexCoord[2].yw).rgb;
	vec3 F = texture2D(rubyTexture, gl_TexCoord[2].zw).rgb;

	// output
	gl_FragColor = vec4(eq(E,A), eq(E,B), eq(E,C), eq(E,F));	
}	
]]></fragment>



// ScaleFX pass 1
<vertex><![CDATA[

uniform vec2 rubyTextureSize;

void main(void) {
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;

	vec2 ps = 1.0/rubyTextureSize;
	float dx = ps.x, dy = ps.y;

	gl_TexCoord[1] = gl_TexCoord[0].xxxy + vec4(  -dx, 0.0, dx,  -dy);	// A, B, C
	gl_TexCoord[2] = gl_TexCoord[0].xxxy + vec4(  -dx, 0.0, dx,  0.0);	// D, E, F
	gl_TexCoord[3] = gl_TexCoord[0].xxxy + vec4(  -dx, 0.0, dx,   dy);	// G, H, I
	gl_TexCoord[4] = gl_TexCoord[0].xxxy + vec4(  -dx, 0.0, dx, 2.0*dy);	// J, K, L
	gl_TexCoord[5] = gl_TexCoord[0].xyyy + vec4(-2.0*dx, -dy,0.0,   dy);	// M, N, O
	gl_TexCoord[6] = gl_TexCoord[0].xyyy + vec4( 2.0*dx, -dy,0.0,   dy);	// P, Q, R
}
]]></vertex>


<fragment scale="1.0" filter="nearest"><![CDATA[
uniform sampler2D rubyTexture;
uniform vec2 rubyTextureSize;

#define SFX_CLR   0.60
float THR = 1.0 - SFX_CLR;

// corner strength
vec4 str(vec4 crn, vec4 ort){
	vec4 tmp;
    tmp.x = (crn.x > THR) ? max(2.0*crn.x - (ort.x + ort.w), 0.0) : 0.0; 
    tmp.y = (crn.y > THR) ? max(2.0*crn.y - (ort.y + ort.x), 0.0) : 0.0; 
    tmp.z = (crn.z > THR) ? max(2.0*crn.z - (ort.z + ort.y), 0.0) : 0.0; 
    tmp.w = (crn.w > THR) ? max(2.0*crn.w - (ort.w + ort.z), 0.0) : 0.0; 
	
	return tmp;
}

// corner dominance at junctions
vec4 dom(vec3 strx, vec3 stry, vec3 strz, vec3 strw){
	vec4 res;
	res.x = max(2.0*strx.y - (strx.x + strx.z), 0.0);
	res.y = max(2.0*stry.y - (stry.x + stry.z), 0.0);
	res.z = max(2.0*strz.y - (strz.x + strz.z), 0.0);
	res.w = max(2.0*strw.y - (strw.x + strw.z), 0.0);
	return res;
}

// necessary but not sufficient junction condition for orthogonal edges
bool clear(vec2 crn, vec4 ort){
	bvec4 tmp;
	tmp.x = (crn.x <= THR || crn.x <= ort.x || crn.x <= ort.w);
	tmp.y = (crn.y <= THR || crn.y <= ort.y || crn.y <= ort.x);
	tmp.z = (crn.x <= THR || crn.x <= ort.z || crn.x <= ort.y);
    tmp.w = (crn.y <= THR || crn.y <= ort.w || crn.y <= ort.z);
	
	return (tmp.x && tmp.y && tmp.z && tmp.w);
}

void main()
{
	/*	grid		metric		pattern

		M A B C P	x y z		x y
		N D E F Q	  o w		w z
		O G H I R
		  J K L
	*/


	// metric data
	vec4 A = texture2D(rubyTexture, gl_TexCoord[1].xw), B = texture2D(rubyTexture, gl_TexCoord[1].yw), C = texture2D(rubyTexture, gl_TexCoord[1].zw);
	vec4 D = texture2D(rubyTexture, gl_TexCoord[2].xw), E = texture2D(rubyTexture, gl_TexCoord[2].yw), F = texture2D(rubyTexture, gl_TexCoord[2].zw);
	vec4 G = texture2D(rubyTexture, gl_TexCoord[3].xw), H = texture2D(rubyTexture, gl_TexCoord[3].yw), I = texture2D(rubyTexture, gl_TexCoord[3].zw);
	vec4 J = texture2D(rubyTexture, gl_TexCoord[4].xw), K = texture2D(rubyTexture, gl_TexCoord[4].yw), L = texture2D(rubyTexture, gl_TexCoord[4].zw);
	vec4 M = texture2D(rubyTexture, gl_TexCoord[5].xy), N = texture2D(rubyTexture, gl_TexCoord[5].xz), O = texture2D(rubyTexture, gl_TexCoord[5].xw);
	vec4 P = texture2D(rubyTexture, gl_TexCoord[6].xy), Q = texture2D(rubyTexture, gl_TexCoord[6].xz), R = texture2D(rubyTexture, gl_TexCoord[6].xw);
	
	// corner strength
	vec4 As = str(vec4(M.z, B.x, D.zx), vec4(A.yw, D.y, M.w));
	vec4 Bs = str(vec4(A.z, C.x, E.zx), vec4(B.yw, E.y, A.w));
	vec4 Cs = str(vec4(B.z, P.x, F.zx), vec4(C.yw, F.y, B.w));
	vec4 Ds = str(vec4(N.z, E.x, G.zx), vec4(D.yw, G.y, N.w));
	vec4 Es = str(vec4(D.z, F.x, H.zx), vec4(E.yw, H.y, D.w));
	vec4 Fs = str(vec4(E.z, Q.x, I.zx), vec4(F.yw, I.y, E.w));
	vec4 Gs = str(vec4(O.z, H.x, J.zx), vec4(G.yw, J.y, O.w));
	vec4 Hs = str(vec4(G.z, I.x, K.zx), vec4(H.yw, K.y, G.w));
	vec4 Is = str(vec4(H.z, R.x, L.zx), vec4(I.yw, L.y, H.w));

	// strength & dominance junctions
	vec4 jSx = vec4(As.z, Bs.w, Es.x, Ds.y), jDx = dom(As.yzw, Bs.zwx, Es.wxy, Ds.xyz);
	vec4 jSy = vec4(Bs.z, Cs.w, Fs.x, Es.y), jDy = dom(Bs.yzw, Cs.zwx, Fs.wxy, Es.xyz);
	vec4 jSz = vec4(Es.z, Fs.w, Is.x, Hs.y), jDz = dom(Es.yzw, Fs.zwx, Is.wxy, Hs.xyz);
	vec4 jSw = vec4(Ds.z, Es.w, Hs.x, Gs.y), jDw = dom(Ds.yzw, Es.zwx, Hs.wxy, Gs.xyz);
	
	// majority vote for ambiguous dominance junctions
	bvec4 jx;
	jx.x = jDx.x != 0.0 && jDx.x + jDx.z > jDx.y + jDx.w;
	jx.y = jDx.y != 0.0 && jDx.y + jDx.w > jDx.z + jDx.x;	
	jx.z = jDx.z != 0.0 && jDx.z + jDx.x > jDx.w + jDx.y;
	jx.w = jDx.w != 0.0 && jDx.w + jDx.y > jDx.x + jDx.z;
	
	bvec4 jy;
	jy.x = jDy.x != 0.0 && jDy.x + jDy.z > jDy.y + jDy.w;
	jy.y = jDy.y != 0.0 && jDy.y + jDy.w > jDy.z + jDy.x;
	jy.z = jDy.z != 0.0 && jDy.z + jDy.x > jDy.w + jDy.y;
	jy.w = jDy.w != 0.0 && jDy.w + jDy.y > jDy.x + jDy.z;

	bvec4 jz;
	jz.x = jDz.x != 0.0 && jDz.x + jDz.z > jDz.y + jDz.w;
	jz.y = jDz.y != 0.0 && jDz.y + jDz.w > jDz.z + jDz.x;	
	jz.z = jDz.z != 0.0 && jDz.z + jDz.x > jDz.w + jDz.y;
	jz.w = jDz.w != 0.0 && jDz.w + jDz.y > jDz.x + jDz.z;	
	
	bvec4 jw;
	jw.x = jDw.x != 0.0 && jDw.x + jDw.z > jDw.y + jDw.w;
	jw.y = jDw.y != 0.0 && jDw.y + jDw.w > jDw.z + jDw.x;
	jw.z = jDw.z != 0.0 && jDw.z + jDw.x > jDw.w + jDw.y;
	jw.w = jDw.w != 0.0 && jDw.w + jDw.y > jDw.x + jDw.z;	

	
	// inject strength without creating new contradictions
	bvec4 res;
	res.x = jx.z || !(jx.y || jx.w) && (jSx.z != 0.0 && (jx.x || jSx.x + jSx.z > jSx.y + jSx.w));
	res.y = jy.w || !(jy.z || jy.x) && (jSy.w != 0.0 && (jy.y || jSy.y + jSy.w > jSy.x + jSy.z));
	res.z = jz.x || !(jz.w || jz.y) && (jSz.x != 0.0 && (jz.z || jSz.x + jSz.z > jSz.y + jSz.w));
	res.w = jw.y || !(jw.x || jw.z) && (jSw.y != 0.0 && (jw.w || jSw.y + jSw.w > jSw.x + jSw.z));

	// single pixel & end of line detection
	res.x = res.x && ((jx.z) || !(res.w && res.y));
	res.y = res.y && ((jy.w) || !(res.x && res.z));
	res.z = res.z && ((jz.x) || !(res.y && res.w));	
	res.w = res.w && ((jw.y) || !(res.z && res.x));
	
	
	// output
	bvec4 clr;
	clr.x = clear(vec2(D.z, E.x), vec4(A.w, E.y, D.wy));
	clr.y = clear(vec2(E.z, F.x), vec4(B.w, F.y, E.wy));
	clr.z = clear(vec2(H.z, I.x), vec4(E.w, I.y, H.wy));
	clr.w = clear(vec2(G.z, H.x), vec4(D.w, H.y, G.wy));

	vec4 low = max(vec4(E.yw, H.y, D.w), THR);
	
	bvec4 hori;
	hori.x = (low.x < max(D.w, A.w)) && clr.x;	// horizontal edges
	hori.y = (low.x < max(E.w, B.w)) && clr.y;	// horizontal edges
	hori.z = (low.z < max(E.w, H.w)) && clr.z;	// horizontal edges
	hori.w = (low.z < max(D.w, G.w)) && clr.w;	// horizontal edges	
	
	bvec4 vert;
	vert.x = (low.w < max(E.y, D.y)) && clr.x;	// vertical edges
	vert.y = (low.y < max(E.y, F.y)) && clr.y;	// vertical edges		
	vert.z = (low.y < max(H.y, I.y)) && clr.z;	// vertical edges	
	vert.w = (low.w < max(H.y, G.y)) && clr.w;	// vertical edges		
	
	bvec4 or   = bvec4(A.w < D.y, B.w <= F.y, H.w < I.y, G.w <= G.y);	// orientation

	gl_FragColor = (vec4(res) + 2.0 * vec4(hori) + 4.0 * vec4(vert) + 8.0 * vec4(or)) / 15.0;
}	
]]></fragment>



// ScaleFX pass 2
<vertex><![CDATA[
uniform vec2 rubyTextureSize;

void main(void) {
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec2 ps = 1.0/rubyTextureSize;
	float dx = ps.x, dy = ps.y;

	gl_TexCoord[1] = gl_TexCoord[0].xxxy + vec4(-dx, -2.0*dx, -3.0*dx,     0.0);	// D, D0, D1
	gl_TexCoord[2] = gl_TexCoord[0].xxxy + vec4( dx,  2.0*dx,  3.0*dx,     0.0);	// F, F0, F1
	gl_TexCoord[3] = gl_TexCoord[0].xyyy + vec4(  0.0,   -dy, -2.0*dy, -3.0*dy);	// B, B0, B1
	gl_TexCoord[4] = gl_TexCoord[0].xyyy + vec4(  0.0,    dy,  2.0*dy,  3.0*dy);	// H, H0, H1	
}
]]></vertex>


<fragment scale="1.0" filter="nearest"><![CDATA[
uniform sampler2D rubyTexture;

vec4 fmod(vec4 a, float b)
{
    vec4 c = fract(abs(a/b))*abs(b);
    return sign(a)*c;
}

// extract first bool4 from vec4 - corners
bvec4 loadCorn(vec4 x){
	return bvec4(floor(fmod(x*15.0 + 0.5, 2.0)));
}

// extract second bool4 from vec4 - horizontal edges
bvec4 loadHori(vec4 x){
	return bvec4(floor(fmod(x*7.5 + 0.25, 2.0)));
}

// extract third bool4 from vec4 - vertical edges
bvec4 loadVert(vec4 x){
	return bvec4(floor(fmod(x*3.75 + 0.125, 2.0)));
}

// extract fourth bool4 from vec4 - orientation
bvec4 loadOr(vec4 x){
	return bvec4(floor(fmod(x*1.875 + 0.0625, 2.0)));
}


void main()
{	
	/*	grid		corners		mids		

		  B		x   y	  	  x
		D E F				w   y
		  H		w   z	  	  z
	*/


	// read data
	vec4 E = texture2D(rubyTexture, gl_TexCoord[0].xy);
	vec4 D = texture2D(rubyTexture, gl_TexCoord[1].xw), D0 = texture2D(rubyTexture, gl_TexCoord[1].yw), D1 = texture2D(rubyTexture, gl_TexCoord[1].zw);
	vec4 F = texture2D(rubyTexture, gl_TexCoord[2].xw), F0 = texture2D(rubyTexture, gl_TexCoord[2].yw), F1 = texture2D(rubyTexture, gl_TexCoord[2].zw);
	vec4 B = texture2D(rubyTexture, gl_TexCoord[3].xy), B0 = texture2D(rubyTexture, gl_TexCoord[3].xz), B1 = texture2D(rubyTexture, gl_TexCoord[3].xw);
	vec4 H = texture2D(rubyTexture, gl_TexCoord[4].xy), H0 = texture2D(rubyTexture, gl_TexCoord[4].xz), H1 = texture2D(rubyTexture, gl_TexCoord[4].xw);

	// extract data
	bvec4 Ec = loadCorn(E), Eh = loadHori(E), Ev = loadVert(E), Eo = loadOr(E);
	bvec4 Dc = loadCorn(D),	Dh = loadHori(D), Do = loadOr(D), D0c = loadCorn(D0), D0h = loadHori(D0), D1h = loadHori(D1);
	bvec4 Fc = loadCorn(F),	Fh = loadHori(F), Fo = loadOr(F), F0c = loadCorn(F0), F0h = loadHori(F0), F1h = loadHori(F1);
	bvec4 Bc = loadCorn(B),	Bv = loadVert(B), Bo = loadOr(B), B0c = loadCorn(B0), B0v = loadVert(B0), B1v = loadVert(B1);
	bvec4 Hc = loadCorn(H),	Hv = loadVert(H), Ho = loadOr(H), H0c = loadCorn(H0), H0v = loadVert(H0), H1v = loadVert(H1);


	// lvl2 mid (left, right / up, down)
	bvec2 lvl2x = bvec2((Ec.x && Eh.y) && Dc.z, (Ec.y && Eh.x) && Fc.w);
	bvec2 lvl2y = bvec2((Ec.y && Ev.z) && Bc.w, (Ec.z && Ev.y) && Hc.x);
	bvec2 lvl2z = bvec2((Ec.w && Eh.z) && Dc.y, (Ec.z && Eh.w) && Fc.x);
	bvec2 lvl2w = bvec2((Ec.x && Ev.w) && Bc.z, (Ec.w && Ev.x) && Hc.y);

	// lvl3 corners (hori, vert)
	bvec2 lvl3x = bvec2(lvl2x.y && (Dh.y && Dh.x) && Fh.z, lvl2w.y && (Bv.w && Bv.x) && Hv.z);
	bvec2 lvl3y = bvec2(lvl2x.x && (Fh.x && Fh.y) && Dh.w, lvl2y.y && (Bv.z && Bv.y) && Hv.w);
	bvec2 lvl3z = bvec2(lvl2z.x && (Fh.w && Fh.z) && Dh.x, lvl2y.x && (Hv.y && Hv.z) && Bv.x);
	bvec2 lvl3w = bvec2(lvl2z.y && (Dh.z && Dh.w) && Fh.y, lvl2w.x && (Hv.x && Hv.w) && Bv.y);

	// lvl4 corners (hori, vert)
	bvec2 lvl4x = bvec2((Dc.x && Dh.y && Eh.x && Eh.y && Fh.x && Fh.y) && (D0c.z && D0h.w), (Bc.x && Bv.w && Ev.x && Ev.w && Hv.x && Hv.w) && (B0c.z && B0v.y));
	bvec2 lvl4y = bvec2((Fc.y && Fh.x && Eh.y && Eh.x && Dh.y && Dh.x) && (F0c.w && F0h.z), (Bc.y && Bv.z && Ev.y && Ev.z && Hv.y && Hv.z) && (B0c.w && B0v.x));
	bvec2 lvl4z = bvec2((Fc.z && Fh.w && Eh.z && Eh.w && Dh.z && Dh.w) && (F0c.x && F0h.y), (Hc.z && Hv.y && Ev.z && Ev.y && Bv.z && Bv.y) && (H0c.x && H0v.w));
	bvec2 lvl4w = bvec2((Dc.w && Dh.z && Eh.w && Eh.z && Fh.w && Fh.z) && (D0c.y && D0h.x), (Hc.w && Hv.x && Ev.w && Ev.x && Bv.w && Bv.x) && (H0c.y && H0v.z));

	// lvl5 mid (left, right / up, down)
	bvec2 lvl5x = bvec2(lvl4x.x && (F0h.x && F0h.y) && (D1h.z && D1h.w), lvl4y.x && (D0h.y && D0h.x) && (F1h.w && F1h.z));
	bvec2 lvl5y = bvec2(lvl4y.y && (H0v.y && H0v.z) && (B1v.w && B1v.x), lvl4z.y && (B0v.z && B0v.y) && (H1v.x && H1v.w));
	bvec2 lvl5z = bvec2(lvl4w.x && (F0h.w && F0h.z) && (D1h.y && D1h.x), lvl4z.x && (D0h.z && D0h.w) && (F1h.x && F1h.y));
	bvec2 lvl5w = bvec2(lvl4x.y && (H0v.x && H0v.w) && (B1v.z && B1v.y), lvl4w.y && (B0v.w && B0v.x) && (H1v.y && H1v.z));

	// lvl6 corners (hori, vert)
	bvec2 lvl6x = bvec2(lvl5x.y && (D1h.y && D1h.x), lvl5w.y && (B1v.w && B1v.x));
	bvec2 lvl6y = bvec2(lvl5x.x && (F1h.x && F1h.y), lvl5y.y && (B1v.z && B1v.y));
	bvec2 lvl6z = bvec2(lvl5z.x && (F1h.w && F1h.z), lvl5y.x && (H1v.y && H1v.z));
	bvec2 lvl6w = bvec2(lvl5z.y && (D1h.z && D1h.w), lvl5w.x && (H1v.x && H1v.w));
	
	vec4 crn;
	crn.x = (Ec.x && Eo.x || lvl3x.x && Eo.y || lvl4x.x && Do.x || lvl6x.x && Fo.y) ? 5.0 : (Ec.x || lvl3x.y && !Eo.w || lvl4x.y && !Bo.x || lvl6x.y && !Ho.w) ? 1.0 : lvl3x.x ? 3.0 : lvl3x.y ? 7.0 : lvl4x.x ? 2.0 : lvl4x.y ? 6.0 : lvl6x.x ? 4.0 : lvl6x.y ? 8.0 : 0.0;
	crn.y = (Ec.y && Eo.y || lvl3y.x && Eo.x || lvl4y.x && Fo.y || lvl6y.x && Do.x) ? 5.0 : (Ec.y || lvl3y.y && !Eo.z || lvl4y.y && !Bo.y || lvl6y.y && !Ho.z) ? 3.0 : lvl3y.x ? 1.0 : lvl3y.y ? 7.0 : lvl4y.x ? 4.0 : lvl4y.y ? 6.0 : lvl6y.x ? 2.0 : lvl6y.y ? 8.0 : 0.0;
	crn.z = (Ec.z && Eo.z || lvl3z.x && Eo.w || lvl4z.x && Fo.z || lvl6z.x && Do.w) ? 7.0 : (Ec.z || lvl3z.y && !Eo.y || lvl4z.y && !Ho.z || lvl6z.y && !Bo.y) ? 3.0 : lvl3z.x ? 1.0 : lvl3z.y ? 5.0 : lvl4z.x ? 4.0 : lvl4z.y ? 8.0 : lvl6z.x ? 2.0 : lvl6z.y ? 6.0 : 0.0;
	crn.w = (Ec.w && Eo.w || lvl3w.x && Eo.z || lvl4w.x && Do.w || lvl6w.x && Fo.z) ? 7.0 : (Ec.w || lvl3w.y && !Eo.x || lvl4w.y && !Ho.w || lvl6w.y && !Bo.x) ? 1.0 : lvl3w.x ? 3.0 : lvl3w.y ? 5.0 : lvl4w.x ? 2.0 : lvl4w.y ? 8.0 : lvl6w.x ? 4.0 : lvl6w.y ? 6.0 : 0.0;

	vec4 mid;
	mid.x = (lvl2x.x &&  Eo.x || lvl2x.y &&  Eo.y || lvl5x.x &&  Do.x || lvl5x.y &&  Fo.y) ? 5.0 : lvl2x.x ? 1.0 : lvl2x.y ? 3.0 : lvl5x.x ? 2.0 : lvl5x.y ? 4.0 : (Ec.x && Dc.z && Ec.y && Fc.w) ? ( Eo.x ?  Eo.y ? 5.0 : 3.0 : 1.0) : 0.0;
	mid.y = (lvl2y.x && !Eo.y || lvl2y.y && !Eo.z || lvl5y.x && !Bo.y || lvl5y.y && !Ho.z) ? 3.0 : lvl2y.x ? 5.0 : lvl2y.y ? 7.0 : lvl5y.x ? 6.0 : lvl5y.y ? 8.0 : (Ec.y && Bc.w && Ec.z && Hc.x) ? (!Eo.y ? !Eo.z ? 3.0 : 7.0 : 5.0) : 0.0;
	mid.z = (lvl2z.x &&  Eo.w || lvl2z.y &&  Eo.z || lvl5z.x &&  Do.w || lvl5z.y &&  Fo.z) ? 7.0 : lvl2z.x ? 1.0 : lvl2z.y ? 3.0 : lvl5z.x ? 2.0 : lvl5z.y ? 4.0 : (Ec.z && Fc.x && Ec.w && Dc.y) ? ( Eo.z ?  Eo.w ? 7.0 : 1.0 : 3.0) : 0.0;
	mid.w = (lvl2w.x && !Eo.x || lvl2w.y && !Eo.w || lvl5w.x && !Bo.x || lvl5w.y && !Ho.w) ? 1.0 : lvl2w.x ? 5.0 : lvl2w.y ? 7.0 : lvl5w.x ? 6.0 : lvl5w.y ? 8.0 : (Ec.w && Hc.y && Ec.x && Bc.z) ? (!Eo.w ? !Eo.x ? 1.0 : 5.0 : 7.0) : 0.0;
	
	gl_FragColor = (crn + 9.0 * mid) / 80.0;	
}	
]]></fragment>



// ScaleFX pass 3
<vertex><![CDATA[
uniform vec2 rubyOrigTextureSize;

void main(void) {

    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;

	vec2 ps = 1.0/rubyOrigTextureSize;
	float dx = ps.x, dy = ps.y;

	gl_TexCoord[1] = gl_TexCoord[0].xxxy + vec4( 0.0, -dx, -2.0*dx,     0.0);	// E, D, D0
	gl_TexCoord[2] = gl_TexCoord[0].xyxy + vec4(dx,   0.0,  2.0*dx,     0.0);	// F, F0
	gl_TexCoord[3] = gl_TexCoord[0].xyxy + vec4( 0.0, -dy,     0.0, -2.0*dy);	// B, B0
	gl_TexCoord[4] = gl_TexCoord[0].xyxy + vec4( 0.0,  dy,     0.0,  2.0*dy);	// H, H0	
}
]]></vertex>


<fragment scale="3.0" filter="nearest"><![CDATA[
uniform sampler2D rubyTexture;
uniform sampler2D rubyOrigTexture;
uniform vec2 rubyTextureSize;

vec4 fmod(vec4 a, float b)
{
    vec4 c = fract(abs(a/b))*abs(b);
    return sign(a)*c;
}

// extract corners
vec4 loadCrn(vec4 x){
	return floor(fmod(x*80.0 + 0.5, 9.0));
}

// extract mids
vec4 loadMid(vec4 x){
	return floor(fmod(x*8.888888 + 0.055555, 9.0));
}


void main()
{	

	/*	grid		corners		mids

		  B		x   y	  	  x
		D E F				w   y
		  H		w   z	  	  z
	*/

	// read data
	vec4 E = texture2D(rubyTexture, gl_TexCoord[0].xy);

	// extract data
	vec4 crn = loadCrn(E);
	vec4 mid = loadMid(E);

	// determine subpixel
	vec2 fp = floor(3.0 * fract(gl_TexCoord[0].xy*rubyTextureSize));
	float  sp = fp.y == 0.0 ? (fp.x == 0.0 ? crn.x : fp.x == 1.0 ? mid.x : crn.y) : (fp.y == 1.0 ? (fp.x == 0.0 ? mid.w : fp.x == 1.0 ? 0.0 : mid.y) : (fp.x == 0.0 ? crn.w : fp.x == 1.0 ? mid.z : crn.z));

	// output coordinate - 0 = E, 1 = D, 2 = D0, 3 = F, 4 = F0, 5 = B, 6 = B0, 7 = H, 8 = H0
	vec2 res = sp == 0.0 ? gl_TexCoord[1].xw : sp == 1.0 ? gl_TexCoord[1].yw : sp == 2.0 ? gl_TexCoord[1].zw : sp == 3.0 ? gl_TexCoord[2].xy : sp == 4.0 ? gl_TexCoord[2].zw : sp == 5.0 ? gl_TexCoord[3].xy : sp == 6.0 ? gl_TexCoord[3].zw : sp == 7.0 ? gl_TexCoord[4].xy : gl_TexCoord[4].zw;

	// ouput	
	gl_FragColor = vec4(texture2D(rubyOrigTexture, res).rgb,1.0);	
}	
]]></fragment>

</shader>
