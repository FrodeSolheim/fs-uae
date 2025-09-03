<?xml version="1.0" encoding="UTF-8"?>
<shader language="GLSL">

<vertex><![CDATA[

void main(void) {
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
}

]]></vertex>

<fragment scale="1.0"><![CDATA[
uniform sampler2D rubyTexture;

void main(void) {   
    vec4 rgb = texture2D(rubyTexture, gl_TexCoord[0].xy);
    float line = float(int(gl_FragCoord.y));
    if (int(mod(line, 2.0)) == 0) {
        gl_FragColor = mix(rgb, vec4(1.0, 1.0, 1.0, 1.0), 0.05);
    }
    else {
        gl_FragColor = rgb * vec4(0.85, 0.85, 0.85, 1.0);
    }
}

]]></fragment>

</shader>
