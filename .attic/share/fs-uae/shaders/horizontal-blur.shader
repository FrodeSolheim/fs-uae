<?xml version="1.0" encoding="UTF-8"?>
<shader language="GLSL">

<fragment><![CDATA[
uniform sampler2D rubyTexture;
uniform vec2 rubyTextureSize;

const float HORIZONTAL_BLUR = 0.8;

void main(void) {   
    vec4 rgb = texture2D(rubyTexture, gl_TexCoord[0].xy);
    
    vec2 coord = gl_TexCoord[0].xy + vec2(-1.0 / rubyTextureSize.x, 0);
    coord.x = max(0, coord.x);

    //if (int(gl_FragCoord.x) == 0) {
    //    rgb2 = rgb;
    //}
    //else {
    //    rgb2 = texture2D(rubyTexture, gl_TexCoord[0].xy + 
    //        vec2(-1.0 / rubyTextureSize.x, 0));
    //}

    vec4 rgb2 = texture2D(rubyTexture, coord);
    rgb = mix(rgb, rgb2, HORIZONTAL_BLUR / 2.0);

    gl_FragColor = rgb;
}

]]></fragment>

</shader>

