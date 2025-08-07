<?xml version="1.0" encoding="UTF-8"?>
<shader language="GLSL">

<fragment scale="1.0"><![CDATA[
uniform sampler2D rubyTexture;

void main(void) {   
    vec4 rgb = texture2D(rubyTexture, gl_TexCoord[0].xy);
    gl_FragColor = vec4(rgb.r * 2.0, rgb.g * 2.0, rgb.b * 2.0, 1.0);
}

]]></fragment>

</shader>

