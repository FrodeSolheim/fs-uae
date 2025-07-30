#ifndef LIBFSEMU_XML_SHADER_H_
#define LIBFSEMU_XML_SHADER_H_

#ifdef WITH_XML_SHADER

int fs_emu_xml_shader_is_enabled();
void fs_emu_xml_shader_init();
int fs_emu_xml_shader_render(int texture, int texture_width,
        int texture_height, int input_width, int input_height,
        int output_width, int output_height, float x1, float y1,
        float x2, float y2, int render_textured_side, float alpha);

#endif

#endif // LIBFSEMU_XML_SHADER_H_
