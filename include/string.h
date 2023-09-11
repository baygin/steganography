#ifndef ST_STRING_H
#define ST_STRING_H

__u_short st_is_alpha_numeric(char *text);
char *st_char_to_binary(char c);
long st_binary_to_char(char *binary);
char *st_binary_to_string(char *binary);
char* st_collect_char(char *string, char c);

#endif // !STRING_H