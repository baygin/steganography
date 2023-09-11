#include "../include/decrypt.h"
#include "../include/cbmp.h"
#include "../include/file.h"
#include "../include/define.h"
#include "../include/string.h"
#include <stdio.h>
#include <string.h>

typedef struct STEGANOGRAPHY_DECRYPT_STRUCT
{
    BMP *bmp;
    char *input;
    char *message;
    char *binary_message;
    __u_long last_x;
    __u_long last_y;
    __u_long width;
    __u_long height;
} st_decrypt;

st_decrypt *st_decrypt_create()
{
    st_decrypt *st = malloc(sizeof(struct STEGANOGRAPHY_DECRYPT_STRUCT));
    st->binary_message = calloc(1024, sizeof(char));
    return st;
}

__u_short st_decrypt_set_input(st_decrypt *st, char *path)
{
    if (path != NULL && file_exists(path))
    {
        st->input = path;

        return ST_NO_ERROR;
    }

    return ST_ERROR_FILE_NOT_EXISTS;
}

static void st_decrypt_decode_message(st_decrypt *st)
{
    st->message = st_binary_to_string(st->binary_message);
}

static __u_short st_encrypt_is_end_x(st_decrypt *st)
{
    return st->last_x == st->width ? ST_OK : ST_NO;
}

static __u_short st_encrypt_is_end_y(st_decrypt *st)
{
    return st->last_y == st->height ? ST_OK : ST_NO;
}

static void st_decrypt_bopen(st_decrypt *st)
{
    st->bmp = bopen(st->input);
    st->width = get_width(st->bmp);
    st->height = get_height(st->bmp);
}

static __u_short st_decrypt_is_end_x(st_decrypt *st)
{
    return st->last_x == st->width ? ST_OK : ST_NO;
}

static __u_short st_decrypt_is_end_y(st_decrypt *st)
{
    return st->last_y == st->height - 1 ? ST_OK : ST_NO;
}

static __u_short st_decrypt_is_next_line(st_decrypt *st)
{
    if (st_decrypt_is_end_x(st) == ST_OK && st_decrypt_is_end_y(st) == ST_NO)
    {
        st->last_x = 0;
        st->last_y++;

        return ST_OK;
    }

    return ST_NO;
}

static __u_short st_decrypt_is_end(st_decrypt *st)
{
    return st_decrypt_is_end_x(st) && ST_OK && st_decrypt_is_end_y(st) ? ST_OK : ST_NO;
}

static void st_decrypt_read_input(st_decrypt *st)
{
    __u_short binary_length = 0;

    st_decrypt_bopen(st);

    while (st_decrypt_is_end(st) == ST_NO)
    {
        st_decrypt_is_next_line(st);

        __u_char r, g, b;
        get_pixel_rgb(st->bmp, st->last_x, st->last_y, &r, &g, &b);

        strcat(st->binary_message, r % 2 == 0 ? "0" : "1");
        strcat(st->binary_message, g % 2 == 0 ? "0" : "1");

        binary_length += 2;

        if (binary_length == 8)
        {
            if (b % 2 != 0)
            {
                break;
            }

            binary_length = 0;
        }
        else
        {
            strcat(st->binary_message, b % 2 == 0 ? "0" : "1");
            binary_length++;
        }

        st->last_x++;
    }
}

char *st_decrypt_get_message(st_decrypt *st)
{
    st_decrypt_read_input(st);

    if (st->binary_message == NULL)
    {
        st_decrypt_read_input(st);
    }

    if (st->message == NULL)
    {
        st_decrypt_decode_message(st);
    }

    return st->message;
}

void *st_decrypt_close(st_decrypt *st)
{
    bclose(st->bmp);
    free(st);
}
