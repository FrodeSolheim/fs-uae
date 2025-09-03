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

const float STRENGTH = 128.0;

void main(void) {
    const float istrength = 255.0 - STRENGTH;
    const float max_brightness = 245.0;

    vec4 rgb = texture2D(rubyTexture, gl_TexCoord[0].xy);
    float line = float(int(gl_FragCoord.y));
    if (int(mod(line, 2.0)) == 0) {
        gl_FragColor = mix(rgb, vec4(1.0, 1.0, 1.0, 1.0), 0.05);
    }
    else {
        float ia = (istrength - (255.0 - max_brightness)) / 255.0 +
                ((rgb.r + rgb.g + rgb.b) / 3.0) / (255.0 / STRENGTH);
        gl_FragColor = rgb * vec4(ia, ia, ia, 1.0);
    }
}

]]></fragment>

</shader>
