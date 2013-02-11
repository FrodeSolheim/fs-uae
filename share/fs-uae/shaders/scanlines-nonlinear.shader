<?xml version="1.0" encoding="UTF-8"?>
<shader language="GLSL">

<vertex><![CDATA[

void main(void) {
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
}

]]></vertex>

<fragment scale="1.0" filter="nearest"><![CDATA[
uniform sampler2D rubyTexture;
uniform vec2 rubyTextureSize;

void main(void) {   
    vec4 rgb = texture2D(rubyTexture, gl_TexCoord[0].xy);
	int line = int(gl_FragCoord.y);
	if (mod(line, 2) == 0) {
		gl_FragColor = mix(rgb, vec4(1.0, 1.0, 1.0, 1.0), 0.05);
	}
	else {
        float ia = (128 - 10) / 255.0 +
        		((rgb.r + rgb.g + rgb.b) / 3.0) / 2.0;
		gl_FragColor = rgb * vec4(ia, ia, ia, 1.0);
	}
}

]]></fragment>

</shader>
