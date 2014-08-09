#ifndef __LEX_H__
#define __LEX_H__

void *lex_current_buffer();
int lex_current_buffer_pos();
void lex_switch_buffer(void *buffer);
void lex_delete_buffer(void *buffer);
void *lex_scan_string_buffer(const char *str);

int yylex();

#endif /* __LEX_H__ */
