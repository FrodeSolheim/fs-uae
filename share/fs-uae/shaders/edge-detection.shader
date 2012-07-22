<?xml version="1.0" encoding="UTF-8"?>
<!--
    Edge shader
    Author: Themaister
    License: Public domain.
-->
<shader language="GLSL">
   <vertex><![CDATA[
     uniform vec2 rubyTextureSize;
 
     void main()
     {
	float x = 0.5 / rubyTextureSize.x;
	float y = 0.5 / rubyTextureSize.y;
	vec2 dg1 = vec2( x, y);
	vec2 dg2 = vec2(-x, y);
	vec2 dx = vec2(x, 0.0);
	vec2 dy = vec2(0.0, y);
 
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1].xy = gl_TexCoord[0].xy - dg1;
	gl_TexCoord[1].zw = gl_TexCoord[0].xy - dy;
	gl_TexCoord[2].xy = gl_TexCoord[0].xy - dg2;
	gl_TexCoord[2].zw = gl_TexCoord[0].xy + dx;
	gl_TexCoord[3].xy = gl_TexCoord[0].xy + dg1;
	gl_TexCoord[3].zw = gl_TexCoord[0].xy + dy;
	gl_TexCoord[4].xy = gl_TexCoord[0].xy + dg2;
	gl_TexCoord[4].zw = gl_TexCoord[0].xy - dx;
     }
    ]]></vertex>
 
   <fragment><![CDATA[
     vec3 grayscale(vec3 color)
     {
	return vec3(dot(color, vec3(0.3, 0.59, 0.11)));
     }
 
     uniform sampler2D rubyTexture;
     uniform vec2 rubyTextureSize;
 
     void main ()
     {
	vec3 c00 = texture2D(rubyTexture, gl_TexCoord[1].xy).xyz;
	vec3 c02 = texture2D(rubyTexture, gl_TexCoord[4].xy).xyz;
	vec3 c11 = texture2D(rubyTexture, gl_TexCoord[0].xy).xyz;
	vec3 c20 = texture2D(rubyTexture, gl_TexCoord[2].xy).xyz;
	vec3 c22 = texture2D(rubyTexture, gl_TexCoord[3].xy).xyz;
 
	vec2 tex = gl_TexCoord[0].xy;
	vec2 texsize = rubyTextureSize;
 
	vec3 first = mix(c00, c20, fract(tex.x * texsize.x + 0.5));
	vec3 second = mix(c02, c22, fract(tex.x * texsize.x + 0.5));
 
	vec3 res = mix(first, second, fract(tex.y * texsize.y + 0.5));
	vec4 final = vec4(5.0 * grayscale(abs(res - c11)), 1.0);
	gl_FragColor = clamp(final, 0.0, 1.0);
     }
    ]]></fragment>
</shader>

