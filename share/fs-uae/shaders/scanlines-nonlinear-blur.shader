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
    vec4 rgb2;
    if (int(gl_FragCoord.x) == 0) {
        rgb2 = rgb;
    }
    else {
        rgb2 = texture2D(rubyTexture, gl_TexCoord[0].xy + 
            vec2(-1.0 / rubyTextureSize.x, 0));
    }
    float line = float(int(gl_FragCoord.y));
    if (int(mod(line, 2.0)) == 0) {
        gl_FragColor = mix(mix(rgb, rgb2, 0.5),
                vec4(1.0, 1.0, 1.0, 1.0), 0.05);
    }
    else {
        float ia = (128.0 - 10.0) / 255.0 +
                ((rgb.r + rgb.g + rgb.b) / 3.0) / 2.0;
        gl_FragColor = mix(rgb, rgb2, 0.5) * vec4(ia, ia, ia, 1.0);
    }
}

]]></fragment>

</shader>
