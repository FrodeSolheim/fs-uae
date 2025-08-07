<?xml version="1.0" encoding="UTF-8"?>
<!--
Lanczos (6tap) scaling

    Copyright (C) 2010 Team XBMC
    http://www.xbmc.org
    Copyright (C) 2011 Stefanos A.
    http://www.opentk.com
    Copyright (C) 2011 BlackStar and DOLLS

This Program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This Program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

-->
<!--
    From these forum posts:

        http://board.byuu.org/viewtopic.php?p=33597#p33597
        http://board.byuu.org/viewtopic.php?p=34426#p34426
-->
<shader language="GLSL">
    <vertex><![CDATA[
        void main()
        {
            gl_TexCoord[0] = gl_MultiTexCoord0;         //center
            gl_Position = ftransform();
        }
    ]]></vertex>

    <fragment><![CDATA[
        #define FIX(c) max(abs(c), 1e-5);

        uniform sampler2D rubyTexture;
        uniform vec2 rubyTextureSize;

        const float PI = 3.1415926535897932384626433832795;

        vec3 weight3(float x)
        {
            const float radius = 3.0;
            vec3 sample = FIX(2.0 * PI * vec3(x - 1.5, x - 0.5, x + 0.5));

            // Lanczos3. Note: we normalize outside this function, so no point in multiplying by radius.
            return /*radius **/ sin(sample) * sin(sample / radius) / (sample * sample);
        }

        vec3 pixel(float xpos, float ypos)
        {
            return texture2D(rubyTexture, vec2(xpos, ypos)).rgb;
        }

        vec3 line(float ypos, vec3 xpos1, vec3 xpos2, vec3 linetaps1, vec3 linetaps2)
        {
            return
                pixel(xpos1.r, ypos) * linetaps1.r +
                pixel(xpos1.g, ypos) * linetaps2.r +
                pixel(xpos1.b, ypos) * linetaps1.g +
                pixel(xpos2.r, ypos) * linetaps2.g +
                pixel(xpos2.g, ypos) * linetaps1.b +
                pixel(xpos2.b, ypos) * linetaps2.b;
        }

        void main()
        {
            vec2 stepxy = 1.0 / rubyTextureSize.xy;
            vec2 pos = gl_TexCoord[0].xy + stepxy * 0.5;
            vec2 f = fract(pos / stepxy);

            vec3 linetaps1   = weight3(0.5 - f.x * 0.5);
            vec3 linetaps2   = weight3(1.0 - f.x * 0.5);
            vec3 columntaps1 = weight3(0.5 - f.y * 0.5);
            vec3 columntaps2 = weight3(1.0 - f.y * 0.5);

            // make sure all taps added together is exactly 1.0, otherwise some
            // (very small) distortion can occur
            float suml = dot(linetaps1, vec3(1.0)) + dot(linetaps2, vec3(1.0));
            float sumc = dot(columntaps1, vec3(1.0)) + dot(columntaps2, vec3(1.0));
            linetaps1 /= suml;
            linetaps2 /= suml;
            columntaps1 /= sumc;
            columntaps2 /= sumc;

            vec2 xystart = (-2.5 - f) * stepxy + pos;
            vec3 xpos1 = vec3(xystart.x, xystart.x + stepxy.x, xystart.x + stepxy.x * 2.0);
            vec3 xpos2 = vec3(xystart.x + stepxy.x * 3.0, xystart.x + stepxy.x * 4.0, xystart.x + stepxy.x * 5.0);

            gl_FragColor = vec4(
                line(xystart.y                 , xpos1, xpos2, linetaps1, linetaps2) * columntaps1.r +
                line(xystart.y + stepxy.y      , xpos1, xpos2, linetaps1, linetaps2) * columntaps2.r +
                line(xystart.y + stepxy.y * 2.0, xpos1, xpos2, linetaps1, linetaps2) * columntaps1.g +
                line(xystart.y + stepxy.y * 3.0, xpos1, xpos2, linetaps1, linetaps2) * columntaps2.g +
                line(xystart.y + stepxy.y * 4.0, xpos1, xpos2, linetaps1, linetaps2) * columntaps1.b +
                line(xystart.y + stepxy.y * 5.0, xpos1, xpos2, linetaps1, linetaps2) * columntaps2.b,
                1.0);
        }
    ]]></fragment>
</shader>
