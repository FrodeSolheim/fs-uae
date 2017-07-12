<?xml version="1.0" encoding="UTF-8"?>
<!--
/*
 * sharp-bilinear
 * License: Public domain
 */
-->
<shader>
<vertex><![CDATA[
void main(void) {
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
]]></vertex>
<fragment scale="4.0" filter="nearest"><![CDATA[
uniform sampler2D rubyTexture;

void main()
{	
	vec3 E = texture2D(rubyTexture, gl_TexCoord[0].xy        ).rgb;
	gl_FragColor = vec4(E,1.0);	
}	
]]></fragment>
</shader>
