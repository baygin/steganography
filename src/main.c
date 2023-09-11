#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/encrypt.h"
#include "../include/decrypt.h"
#include "../include/define.h"

int main(int argc, char *argv[])
{
    if (strcmp(argv[1], "encode") == 0)
    {
        st_encrypt *st = st_encrypt_create();

        if (st_encrypt_set_input(st, argv[2]) != ST_NO_ERROR)
        {
            printf("Input file doesn't exists\n");
            exit(EXIT_FAILURE);
        }

        st_encrypt_set_output(st, argv[3]);

        if (st_encrypt_set_message(st, argv[4]) != ST_NO_ERROR)
        {
            printf("Message error\n");
            exit(EXIT_FAILURE);
        }

        __u_short error = st_encrypt_save(st);

        if (error != ST_NO_ERROR)
        {
            printf("Error: %d\n", error);
            exit(EXIT_FAILURE);
        }

        st_encrypt_close(st);

        printf("File saved\n");
    }
    else if (strcmp(argv[1], "decode") == 0)
    {
        st_decrypt *st = st_decrypt_create();

        st_decrypt_set_input(st, argv[2]);

        char *message = st_decrypt_get_message(st);

        printf("The secret message: %s\n", message);

        st_decrypt_close(st);
    }
}
