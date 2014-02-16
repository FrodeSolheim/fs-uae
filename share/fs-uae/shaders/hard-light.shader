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

const float HARD_LIGHT = 0.5;
const float MIX_ALPHA = 0.4;

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

    vec4 rgb3 = mix(rgb, rgb2, MIX_ALPHA);
    vec4 rgb4;

    for (int i = 0; i < 3; i++) {
        if (rgb3[i] > 0.5) {
            rgb4[i] = 1.0 - 2.0 * (1.0 - rgb3[i]) * (1.0 - rgb3[i]);
        }
        else {
            rgb4[i] = 2.0 * rgb3[i] * rgb3[i];
        }
        
        //rgb4[i] = pow(rgb4[i], 1.0 / 1.13);
    }
    rgb4.a = 1.0;

    gl_FragColor = rgb4 * HARD_LIGHT + rgb3 * (1.0 - HARD_LIGHT);

    //gl_FragColor = rgb4;
}

]]></fragment>

<fragment outscale="1.0"><![CDATA[
uniform sampler2D rubyTexture;

const float LIGHTNESS_GAMMA = 1.0;
const float STRENGTH = 128.0;
const float STRENGTH2 = 200.0;

float Hue_2_RGB(float v1, float v2, float vH )
{
	float ret;
   if ( vH < 0.0 )
     vH += 1.0;
   if ( vH > 1.0 )
     vH -= 1.0;
   if ( ( 6.0 * vH ) < 1.0 )
     ret = ( v1 + ( v2 - v1 ) * 6.0 * vH );
   else if ( ( 2.0 * vH ) < 1.0 )
     ret = ( v2 );
   else if ( ( 3.0 * vH ) < 2.0 )
     ret = ( v1 + ( v2 - v1 ) * ( ( 2.0 / 3.0 ) - vH ) * 6.0 );
   else
     ret = v1;
   return ret;
}

void main(void) {   
    const float istrength = 255.0 - STRENGTH;
    const float istrength2 = 255.0 - STRENGTH2;
    const float max_brightness = 245.0;

    vec4 rgb = texture2D(rubyTexture, gl_TexCoord[0].xy);

    float Cmax, Cmin;
    float D;
    float H, S, L;
    float R, G, B;

    R = rgb.r;
    G = rgb.g;
    B = rgb.b;

    Cmax = max (R, max (G, B));
    Cmin = min (R, min (G, B));

// calculate lightness
    L = (Cmax + Cmin) / 2.0;
    if (Cmax == Cmin) { // it's grey
        H = 0.0; // it's actually undefined
        S = 0.0;
    }
    else {
        D = Cmax - Cmin; // we know D != 0 so we cas safely divide by it// calculate lightness

// calculate Saturation
    if (L < 0.5)
    {
      S = D / (Cmax + Cmin);
    }
    else
    {
      S = D / (2.0 - (Cmax + Cmin));
    }

// calculate Hue
    if (R == Cmax)
    {
      H = (G - B) / D;
		} else {
      if (G == Cmax)
      {
      	 H = 2.0 + (B - R) /D;
      }
      else
      {
        H = 4.0 + (R - G) / D;
      }
   	}

    H = H / 6.0;


	}

    float line = float(int(gl_FragCoord.y));
    if (int(mod(line, 3.0)) < 1) {
        //float ia = (istrength - (255.0 - max_brightness)) / 255.0 +
        //        ((rgb.r + rgb.g + rgb.b) / 3.0) / (255.0 / STRENGTH);
        //gl_FragColor = rgb * vec4(ia, ia, ia, 1.0);

        float ia = (istrength - (255.0 - max_brightness)) / 255.0 +
                (L) / (255.0 / STRENGTH);
        L = L * ia;
    }
    else if (int(mod(line, 3.0)) < 2) {
        //float ia = (istrength2 - (255.0 - max_brightness)) / 255.0 +
        //        ((rgb.r + rgb.g + rgb.b) / 3.0) / (255.0 / STRENGTH2);
        //gl_FragColor = rgb * vec4(ia, ia, ia, 1.0);
        //L = L * 0.95 + 0.05;

        float ia = (istrength - (255.0 - max_brightness)) / 255.0 +
                (L) / (255.0 / STRENGTH2);
        L = L * ia;
    }
    else {
        //gl_FragColor = mix(rgb,
        //        vec4(1.0, 1.0, 1.0, 1.0), 0.05);
        L = L * 0.95 + 0.05;
    }

    // S = S * 3.0;
    L = pow(L, 1.0 / LIGHTNESS_GAMMA);

if (H < 0.0)
{
	H = H + 1.0;
}

// clamp H,S and L to [0..1]

H = clamp(H, 0.0, 1.0);
S = clamp(S, 0.0, 1.0);
L = clamp(L, 0.0, 1.0);

float var_2, var_1;

if (S == 0.0)
{
   R = L;
   G = L;
   B = L;
}
else
{
   if ( L < 0.5 )
   {
   var_2 = L * ( 1.0 + S );
   }
   else
   {
   var_2 = ( L + S ) - ( S * L );
   }

   var_1 = 2.0 * L - var_2;

   R = Hue_2_RGB( var_1, var_2, H + ( 1.0 / 3.0 ) );
   G = Hue_2_RGB( var_1, var_2, H );
   B = Hue_2_RGB( var_1, var_2, H - ( 1.0 / 3.0 ) );
}

/*
rgb[0] = R;
rgb[1] = G;
rgb[2] = B;
for (int i = 0; i < 3; i++) {
    if (rgb[i] > 0.5) {
    	rgb[i] = 1.0 - 2 * (1.0 - rgb[i]) * (1.0 - rgb[i]);
    }
    else {
    	rgb[i] = 2 * rgb[i] * rgb[i];
    }
}
*/
 //gl_FragColor = vec4(rgb[0], rgb[1], rgb[2], rgb.a);

 gl_FragColor = vec4(R, G, B, rgb.a);

}

]]></fragment>

</shader>
