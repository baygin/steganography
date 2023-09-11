#include "../include/file.h"
#include "../include/define.h"
#include <stdio.h>

__u_short file_exists(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file)
    {
        fclose(file);
        return ST_OK;
    }
    
    return ST_NO;
}