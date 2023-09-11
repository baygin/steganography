#include <stdlib.h>

#include "../include/encrypt.h"
#include "../include/string.h"
#include "../include/file.h"
#include "../include/define.h"
#include "../include/cbmp.h"

#include <string.h>
#include <stdio.h>

typedef struct STEGANOGRAPHY_ENCRYPT_STRUCT
{
    BMP *bmp;
    char *input;
    char *output;
    char *message;
    __u_long message_length;
    __u_long last_x;
    __u_long last_y;
    __u_long width;
    __u_long height;
} st_encrypt;

st_encrypt *st_encrypt_create()
{
    st_encrypt *st = malloc(sizeof(struct STEGANOGRAPHY_ENCRYPT_STRUCT));

    return st;
}

__u_short st_encrypt_set_input(st_encrypt *st, char *path)
{
    if (path != NULL && file_exists(path))
    {
        st->input = path;
        return ST_NO_ERROR;
    }

    return ST_ERROR_FILE_NOT_EXISTS;
}

__u_short st_encrypt_set_output(st_encrypt *st, char *path)
{
    st->output = path;

    return ST_NO_ERROR;
}

__u_short st_encrypt_set_message(st_encrypt *st, char *message)
{
    if (st_is_alpha_numeric(message) == ST_NO)
    {
        return ST_ERROR_TEXT_MUST_ASCII;
    }

    st->message = message;
    st->message_length = strlen(st->message);

    return ST_NO_ERROR;
}

static void st_encrypt_bopen(st_encrypt *st)
{
    st->bmp = bopen(st->input);
    st->width = get_width(st->bmp);
    st->height = get_height(st->bmp);
}

static __u_short st_encrypt_check_bmp_error(st_encrypt *st)
{
    if (st->bmp == NULL)
    {
        return ST_ERROR_BMP;
    }

    __u_long writable_area = ((st->bmp->width * st->bmp->height) / 9) - 1;

    if (writable_area < strlen(st->message))
    {
        return ST_ERROR_TEXT_MAX_SIZE;
    }

    return ST_NO_ERROR;
}

static __u_short st_encrypt_check_error(st_encrypt *st)
{
    if (st->output == NULL)
    {
        return ST_ERROR_NO_OUTPUT;
    }

    if (st->input == NULL)
    {
        return ST_ERROR_NO_INPUT;
    }

    if (st->message == NULL)
    {
        return ST_ERROR_NO_TEXT;
    }

    return ST_NO_ERROR;
}

static __u_short st_encrypt_is_end_x(st_encrypt *st)
{
    return st->last_x == st->width ? ST_OK : ST_NO;
}

static __u_short st_encrypt_is_end_y(st_encrypt *st)
{
    return st->last_y == st->height - 1 ? ST_OK : ST_NO;
}

static __u_short st_encrypt_roll(__u_short color, int type)
{
    __u_short mod = color % 2;

    if (type == ST_ROLL_ODD)
    {
        if (mod == 0)
        {
            color++;
        }
    }
    else if (type == ST_ROLL_EVEN)
    {
        if (mod != 0)
        {
            color--;
        }
    }

    return color;
}

static __u_short st_encrypt_binary_roll(char c, __u_short color)
{
    if (c == '0')
    {
        color = st_encrypt_roll(color, ST_ROLL_EVEN);
    }
    else if (c == '1')
    {
        color = st_encrypt_roll(color, ST_ROLL_ODD);
    }

    return color;
}

static __u_short st_encrypt_is_next_line(st_encrypt *st)
{
    if (st_encrypt_is_end_x(st) == ST_OK && st_encrypt_is_end_y(st) == ST_NO)
    {
        st->last_x = 0;
        st->last_y++;

        return ST_OK;
    }

    return ST_NO;
}

static void st_encrypt_write_char(st_encrypt *st, char *binary, __u_short end)
{
    __u_short binary_index = 0;

    for (size_t i = 0; i < 3; i++)
    {
        st_encrypt_is_next_line(st);

        __u_char r, g, b;
        get_pixel_rgb(st->bmp, st->last_x, st->last_y, &r, &g, &b);

        r = st_encrypt_binary_roll(binary[binary_index++], r);
        g = st_encrypt_binary_roll(binary[binary_index++], g);

        if (i == 2)
        {
            if (end)
            {
                b = st_encrypt_roll(b, ST_ROLL_ODD);
            }
            else
            {
                b = st_encrypt_roll(b, ST_ROLL_EVEN);
            }
        }
        else
        {
            b = st_encrypt_binary_roll(binary[binary_index++], b);
        }

        set_pixel_rgb(st->bmp, st->last_x, st->last_y, r, g, b);

        st->last_x++;
    }
}

static void st_encrypt_write(st_encrypt *st)
{
    for (size_t i = 0; i < st->message_length; i++)
    {
        char *binary = st_char_to_binary(st->message[i]);
        st_encrypt_write_char(st, binary, i == st->message_length - 1 ? ST_OK : ST_NO);
    }
}

__u_short st_encrypt_save(st_encrypt *st)
{
    __u_short error = st_encrypt_check_error(st);

    if (error != ST_NO_ERROR)
    {
        return error;
    }

    st_encrypt_bopen(st);

    __u_short bmp_error = st_encrypt_check_bmp_error(st);

    if (bmp_error != ST_NO_ERROR)
    {
        return bmp_error;
    }

    st_encrypt_write(st);
    bwrite(st->bmp, st->output);

    return ST_NO_ERROR;
}

void st_encrypt_close(st_encrypt *st)
{
    bclose(st->bmp);
    free(st);
}