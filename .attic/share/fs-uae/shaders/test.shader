<?xml version="1.0" encoding="UTF-8"?>
<shader language="GLSL">

<fragment scale="1.0" filter="nearest"><![CDATA[
uniform sampler2D rubyTexture;
uniform vec2 rubyTextureSize;

const float HARD_LIGHT = 0.4; // 0.4 is good
const float HORIZONTAL_BLUR = 0.0;

void main(void) {   
    vec4 color = texture2D(rubyTexture, gl_TexCoord[0].xy);
    if (int(gl_FragCoord.x) > 0) {
        vec4 blend_color = texture2D(rubyTexture, gl_TexCoord[0].xy + 
            vec2(-1.0 / rubyTextureSize.x, 0));
        color = mix(color, blend_color, HORIZONTAL_BLUR / 2.0);
    }

    vec4 hard_light;
    for (int i = 0; i < 3; i++) {
        if (color[i] > 0.5) {
            hard_light[i] = 1.0 - 2.0 * (1.0 - color[i]) * (1.0 - color[i]);
        }
        else {
            hard_light[i] = 2.0 * color[i] * color[i];
        }
    }
    hard_light.a = 1.0;

    gl_FragColor = hard_light * HARD_LIGHT + color * (1.0 - HARD_LIGHT);
}

]]></fragment>

<fragment><![CDATA[
uniform sampler2D rubyTexture;
uniform vec2 rubyOutputSize;

const float LIGHTNESS_GAMMA = 1.3;

float hue_to_rgb(float v1, float v2, float vH) {
    float ret;
    if (vH < 0.0) vH += 1.0;
    if (vH > 1.0) vH -= 1.0;
    if ((6.0 * vH) < 1.0) ret = (v1 + (v2 - v1) * 6.0 * vH);
    else if ((2.0 * vH) < 1.0) ret = (v2);
    else if ((3.0 * vH) < 2.0)
        ret = (v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0);
    else ret = v1;
    return ret;
}

void main(void) {   
    vec4 color = texture2D(rubyTexture, gl_TexCoord[0].xy);

    float line = float(int(gl_FragCoord.y));
	float strength = 0.0;

	if (rubyOutputSize.y >= 960.0) {
        int step = int(mod(line, 4.0));

		if (step == 0) strength = 0.25;
		else if (step == 1) strength = 0.35;
	    // else if (step == 2) strength = 0.25;
	    // else if (step == 3) strength = 0.0;
	    // else if (step == 4) strength = 0.0;
	}
	else {
        int step = int(mod(line, 3.0));

		if (step == 0) strength = 0.25;
		else if (step == 1) strength = 0.35;
	    // else if (step == 3) strength = 0.0;
	}

    float d, h, s, l;
    float cmax = max(color.r, max(color.g, color.b));
    float cmin = min(color.r, min(color.g, color.b));
    l = (cmax + cmin) / 2.0;
    if (cmax == cmin) {
        h = 0.0;
        s = 0.0;
    }
    else {
        d = cmax - cmin;
        if (l < 0.5) s = d / (cmax + cmin);
        else s = d / (2.0 - (cmax + cmin));
        if (color.r == cmax) h = (color.g - color.b) / d;
        else {
            if (color.g == cmax) h = 2.0 + (color.b - color.r) / d;
            else h = 4.0 + (color.r - color.g) / d;
        }
        h = h / 6.0;
    }
	
	l = pow(l * (1.0 - strength * 0.1), 1.0 / (1.0 - strength));
	// s = pow(s * (1.0 - strength * 0.1), 1.0 / (1.0 - strength));

    l = pow(l, 1.0 / LIGHTNESS_GAMMA);

    if (h < 0.0) h = h + 1.0;
    h = clamp(h, 0.0, 1.0);
    s = clamp(s, 0.0, 1.0);
    l = clamp(l, 0.0, 1.0);

    float var_2, var_1;

    if (s == 0.0) {
       color.r = l;
       color.g = l;
       color.b = l;
    }
    else {
        if (l < 0.5) var_2 = l * (1.0 + s);
        else var_2 = (l + s) - (s * l);
        var_1 = 2.0 * l - var_2;

        color.r = hue_to_rgb(var_1, var_2, h + (1.0 / 3.0));
        color.g = hue_to_rgb(var_1, var_2, h);
        color.b = hue_to_rgb(var_1, var_2, h - (1.0 / 3.0));
    }

	gl_FragColor = color;
}

]]></fragment>

</shader>

