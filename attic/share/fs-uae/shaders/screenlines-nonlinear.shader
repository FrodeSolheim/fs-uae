<?xml version="1.0" encoding="UTF-8"?>
<shader language="GLSL">

<fragment><![CDATA[
uniform sampler2D rubyTexture;
uniform vec2 rubyOutputSize;

void main(void) {   
    vec4 color = texture2D(rubyTexture, gl_TexCoord[0].xy);

    float line = float(int(gl_FragCoord.y));

	float strength = 0.0;
	if (rubyOutputSize.y >= 960.0) {
        int step = int(mod(line, 5.0));

		if (step == 0) strength = 0.25;
		else if (step == 1) strength = 0.35;
	    else if (step == 2) strength = 0.25;
	    // else if (step == 3) strength = 0.0;
	    // else if (step == 4) strength = 0.0;
	}
	else {
        int step = int(mod(line, 3.0));

		if (step == 0) strength = 0.25;
		else if (step == 1) strength = 0.35;
	    // else if (step == 3) strength = 0.0;
	}

	color.r = pow(color.r * (1.0 - strength * 0.1), 1.0 / (1.0 - strength));
	color.g = pow(color.g * (1.0 - strength * 0.1), 1.0 / (1.0 - strength));
	color.b = pow(color.b * (1.0 - strength * 0.1), 1.0 / (1.0 - strength));	
	
	gl_FragColor = color;
}

]]></fragment>

</shader>
