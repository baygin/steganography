#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/string.h"
#include "../include/define.h"

__u_short st_is_alpha_numeric(char *text)
{
    __u_short status = ST_OK;

    for (size_t i = 0; i < strlen(text); i++)
    {
        if (text[i] != ' ' && isalnum(text[i]) == 0)
        {
            status = ST_NO;
            break;
        }
    }

    return status;
}

/**
 * @ref https://stackoverflow.com/questions/7863499/conversion-of-char-to-binary-in-c
 */

char *st_char_to_binary(char c)
{
    char *binary = malloc(sizeof(char) + 1);

    binary[0] = '\0';

    for (int j = 7; j >= 0; --j)
    {
        if (c & (1 << j))
        {
            strcat(binary, "1");
        }
        else
        {
            strcat(binary, "0");
        }
    }

    return binary;
}

long st_binary_to_char(char *binary)
{
    return strtol(binary, 0, 2);
}

static char *st_char_to_string(char c)
{
    char *string = calloc(2, sizeof(char));
    string[0] = c;
    string[1] = '\0';

    return string;
}

char *st_collect_char(char *string, char c)
{
    return strcat(string, st_char_to_string(c));
}

char *st_binary_to_string(char *binary)
{
    if(binary == NULL) {
        return binary;
    }

    size_t binary_length = strlen(binary);
    char *single_binary = malloc(sizeof(char) * 9);
    char *string = calloc(((binary_length / 8) + 1), sizeof(char));

    for (size_t i = 0; i < binary_length; i++)
    {
        st_collect_char(single_binary, binary[i]);

        if ((i + 1) % 8 == 0)
        {
            char c = st_binary_to_char(single_binary);
            st_collect_char(string, c);
            memset(single_binary, 0, sizeof(char) * 9);
        }
    }

    return string;
}