<?xml version="1.0" encoding="UTF-8"?>
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
   Hyllian's CRT Shader
  
   Copyright (C) 2011-2016 Hyllian - sergiogdb@gmail.com

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


<fragment outscale="1.0" filter="nearest"><![CDATA[
uniform sampler2D rubyTexture;
uniform vec2 rubyTextureSize;
uniform vec2 rubyOutputSize;
uniform vec2 rubyInputSize;

#define PHOSPHOR 1.0
#define VSCANLINES 0.0
#define InputGamma 2.5
#define OutputGamma 2.2
#define SHARPNESS 1.0
#define COLOR_BOOST 1.5
#define RED_BOOST 1.0
#define GREEN_BOOST 1.0
#define BLUE_BOOST 1.0
#define SCANLINES_STRENGTH 0.50
#define BEAM_MIN_WIDTH 0.86
#define BEAM_MAX_WIDTH 1.0
#define CRT_ANTI_RINGING 0.8  

#define GAMMA_IN(color)     pow(color, vec3(InputGamma, InputGamma, InputGamma))
#define GAMMA_OUT(color)    pow(color, vec3(1.0 / OutputGamma, 1.0 / OutputGamma, 1.0 / OutputGamma))


// Horizontal cubic filter.

// Some known filters use these values:

//    B = 0.0, C = 0.0  =>  Hermite cubic filter.
//    B = 1.0, C = 0.0  =>  Cubic B-Spline filter.
//    B = 0.0, C = 0.5  =>  Catmull-Rom Spline filter. This is the default used in this shader.
//    B = C = 1.0/3.0   =>  Mitchell-Netravali cubic filter.
//    B = 0.3782, C = 0.3109  =>  Robidoux filter.
//    B = 0.2620, C = 0.3690  =>  Robidoux Sharp filter.
//    B = 0.36, C = 0.28  =>  My best config for ringing elimination in pixel art (Hyllian).


// For more info, see: http://www.imagemagick.org/Usage/img_diagrams/cubic_survey.gif

// Change these params to configure the horizontal filter.
const float  B =  0.0; 
const float  C =  0.5;  

mat4 invX = mat4(            (-B - 6.0*C)/6.0,         (3.0*B + 12.0*C)/6.0,     (-3.0*B - 6.0*C)/6.0,             B/6.0,
                                        (12.0 - 9.0*B - 6.0*C)/6.0, (-18.0 + 12.0*B + 6.0*C)/6.0,                      0.0, (6.0 - 2.0*B)/6.0,
                                       -(12.0 - 9.0*B - 6.0*C)/6.0, (18.0 - 15.0*B - 12.0*C)/6.0,      (3.0*B + 6.0*C)/6.0,             B/6.0,
                                                   (B + 6.0*C)/6.0,                           -C,                      0.0,               0.0); 
 
 
void main(void) {   
    vec3 color;

    vec2 TextureSize = vec2(SHARPNESS*rubyTextureSize.x, rubyTextureSize.y);

    vec2 dx = mix(vec2(1.0/TextureSize.x, 0.0), vec2(0.0, 1.0/TextureSize.y), VSCANLINES);
    vec2 dy = mix(vec2(0.0, 1.0/TextureSize.y), vec2(1.0/TextureSize.x, 0.0), VSCANLINES);

    vec2 pix_coord = gl_TexCoord[0].xy*TextureSize + vec2(-0.5, 0.5);

    vec2 tc = mix((floor(pix_coord) + vec2(0.5, 0.5))/TextureSize, (floor(pix_coord) + vec2(1.0, -0.5))/TextureSize, VSCANLINES);

    vec2 fp = mix(fract(pix_coord), fract(pix_coord.yx), VSCANLINES);

    vec3 c00 = GAMMA_IN(texture2D(rubyTexture, tc     - dx - dy).xyz);
    vec3 c01 = GAMMA_IN(texture2D(rubyTexture, tc          - dy).xyz);
    vec3 c02 = GAMMA_IN(texture2D(rubyTexture, tc     + dx - dy).xyz);
    vec3 c03 = GAMMA_IN(texture2D(rubyTexture, tc + 2.0*dx - dy).xyz);
    vec3 c10 = GAMMA_IN(texture2D(rubyTexture, tc     - dx     ).xyz);
    vec3 c11 = GAMMA_IN(texture2D(rubyTexture, tc              ).xyz);
    vec3 c12 = GAMMA_IN(texture2D(rubyTexture, tc     + dx     ).xyz);
    vec3 c13 = GAMMA_IN(texture2D(rubyTexture, tc + 2.0*dx     ).xyz);

    // Get min/max samples
    vec3 min_sample = min(min(c01, c11), min(c02, c12));
    vec3 max_sample = max(max(c01, c11), max(c02, c12));

    mat4x3 color_matrix0 = mat4x3(c00, c01, c02, c03);
    mat4x3 color_matrix1 = mat4x3(c10, c11, c12, c13);
    
    vec4 invX_Px    = vec4(fp.x*fp.x*fp.x, fp.x*fp.x, fp.x, 1.0) * invX;
    vec3 color0     = color_matrix0 * invX_Px;
    vec3 color1     = color_matrix1 * invX_Px;

    // Anti-ringing
    vec3 aux    = color0;
    color0      = clamp(color0, min_sample, max_sample);
    color0      = mix(aux, color0, CRT_ANTI_RINGING);
    aux         = color1;
    color1      = clamp(color1, min_sample, max_sample);
    color1      = mix(aux, color1, CRT_ANTI_RINGING);

    float pos0 = fp.y;
    float pos1 = 1 - fp.y;

    vec3 lum0 = mix(vec3(BEAM_MIN_WIDTH), vec3(BEAM_MAX_WIDTH), color0);
    vec3 lum1 = mix(vec3(BEAM_MIN_WIDTH), vec3(BEAM_MAX_WIDTH), color1);

    vec3 d0 = clamp(pos0/(lum0 + 0.0000001), 0.0, 1.0);
    vec3 d1 = clamp(pos1/(lum1 + 0.0000001), 0.0, 1.0);

    d0 = exp(-10.0*SCANLINES_STRENGTH*d0*d0);
    d1 = exp(-10.0*SCANLINES_STRENGTH*d1*d1);

    color = clamp(color0*d0 + color1*d1, 0.0, 1.0);            

    color *= COLOR_BOOST*vec3(RED_BOOST, GREEN_BOOST, BLUE_BOOST);

    float mod_factor = mix(gl_TexCoord[0].x * rubyOutputSize.x * rubyTextureSize.x / rubyInputSize.x, gl_TexCoord[0].y * rubyOutputSize.y * rubyTextureSize.y / rubyInputSize.y, VSCANLINES);

    vec3 dotMaskWeights = mix(
        vec3(1.0, 0.7, 1.0),
        vec3(0.7, 1.0, 0.7),
        floor(mod(mod_factor, 2.0))
    );

    color.rgb *= mix(vec3(1.0), dotMaskWeights, PHOSPHOR);

    color  = GAMMA_OUT(color);

    gl_FragColor = vec4(color, 1.0); 
}

]]></fragment>


</shader>

