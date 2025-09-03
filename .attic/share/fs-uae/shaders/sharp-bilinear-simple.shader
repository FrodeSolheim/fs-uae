<?xml version="1.0" encoding="UTF-8"?>
<!--
   Author: rsn8887 (based on TheMaister)
   License: Public domain
   This is an integer prescale filter that should be combined
   with a bilinear hardware filtering (GL_BILINEAR filter or some such) to achieve
   a smooth scaling result with minimum blur. This is good for pixelgraphics
   that are scaled by non-integer factors.
   
   The prescale factor and texel coordinates are precalculated
   in the vertex shader for speed.
-->
<shader language="GLSL">
    <vertex><![CDATA[
        uniform vec2 rubyInputSize;
        uniform vec2 rubyOutputSize;
        uniform vec2 rubyTextureSize;
        varying vec2 texCoord;
        varying vec2 precalc_texel;
        varying vec2 precalc_scale;

        void main()
        {
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                texCoord = gl_MultiTexCoord0.xy;
                precalc_texel = texCoord * rubyTextureSize;
                // The small number compensates for possible floating point rounding errors
                // when scaling by exact integer mutiples, e.g. 540/1080=1.99999994
                precalc_scale = max(floor(rubyOutputSize / rubyInputSize + 0.0000002), vec2(1.0, 1.0));
        }
    ]]></vertex>

    <fragment filter="linear"><![CDATA[

        uniform sampler2D rubyTexture;
        uniform vec2 rubyInputSize;
        uniform vec2 rubyOutputSize;
        uniform vec2 rubyTextureSize;
        varying vec2 precalc_texel;
        varying vec2 precalc_scale;
        varying vec2 texCoord;

        void main()
        {
           vec2 texel = precalc_texel;
           vec2 scale = precalc_scale;

           vec2 texel_floored = floor(texel);
           vec2 s = fract(texel);
           vec2 region_range = 0.5 - 0.5 / scale;

           // Figure out where in the texel to sample to get correct pre-scaled bilinear.
           // Uses the hardware bilinear interpolator to avoid having to sample 4 times manually.

           vec2 center_dist = s - 0.5;
           vec2 f = (center_dist - clamp(center_dist, -region_range, region_range)) * scale + 0.5;

           vec2 mod_texel = texel_floored + f;

           gl_FragColor = vec4(texture2D(rubyTexture, mod_texel / rubyTextureSize).rgb, 1.0);
        }
    ]]></fragment>
</shader>
