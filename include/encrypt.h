#ifndef ST_ENCRYPT_H
#define ST_ENCRYPT_H

#include <stdlib.h>

typedef struct STEGANOGRAPHY_ENCRYPT_STRUCT st_encrypt;

st_encrypt *st_encrypt_create();
__u_short st_encrypt_set_input(st_encrypt *st, char *path);
__u_short st_encrypt_set_output(st_encrypt *st, char *path);
__u_short st_encrypt_set_message(st_encrypt *st, char *message);
__u_short st_encrypt_save(st_encrypt *st);
void st_encrypt_close(st_encrypt *st);

#endif // !ST_ENCRYPT_H