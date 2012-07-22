<?xml version="1.0" encoding="UTF-8"?>
<shader language="GLSL">
  <fragment filter="linear"><![CDATA[
    uniform sampler2D rubyTexture;
    uniform vec2 rubyInputSize;
    uniform vec2 rubyTextureSize;

    // Tweak this parameter for more / less distortion
    #define distortion 0.2

    vec2 radialDistortion(vec2 coord) {
      coord *= rubyTextureSize / rubyInputSize;
      vec2 cc = coord - vec2(0.5);
      float dist = dot(cc, cc) * distortion;
      return (coord + cc * (1.0 + dist) * dist) * rubyInputSize / rubyTextureSize;
    }

    void main(void) {
      gl_FragColor = texture2D(rubyTexture, radialDistortion(gl_TexCoord[0].xy));
    }
  ]]></fragment>
</shader>
