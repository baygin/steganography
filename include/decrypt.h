#ifndef ST_DECRYPT_H
#define ST_DECRYPT_H

#include <bits/types.h>

typedef struct STEGANOGRAPHY_DECRYPT_STRUCT st_decrypt;

st_decrypt *st_decrypt_create();
__u_short st_decrypt_set_input(st_decrypt *st, char *path);
char *st_decrypt_get_message(st_decrypt *st);
void *st_decrypt_close(st_decrypt *st);

#endif // !ST_DECRYPT_H