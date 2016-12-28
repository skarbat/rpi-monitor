//-------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Copyright (c) 2016 Albert Casals
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------


// TODO: change asserts for recoverable error handlers

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "monitor.h"


/*
 *  monitor_collect()
 *
 *  Executes monitor process to collect system information.
 *  Returns array of strings each containing one data item.
 */
char **monitor_collect(const char *monitor_program)
{
    char **data_strings=NULL;
    char buffer[128];
    int line_count=0;

    FILE *fpmonitor = popen(monitor_program, "r");
    if (fpmonitor) {

        // read line entries and collect them to list of strings
        memset(buffer, 0x00, sizeof(buffer));
        while (fgets(buffer, sizeof(buffer), fpmonitor)) {
            data_strings = realloc(data_strings, line_count * sizeof(char *) + sizeof (char *));
            if (data_strings) {
                data_strings[line_count++] = strdup(buffer);
            }
        }

        // add a mark to end the list
        if (line_count) {
            data_strings = realloc(data_strings, line_count * sizeof(char *) + sizeof (char *));
            if (data_strings) {
                data_strings[line_count] = NULL;
            }
        }

        fclose(fpmonitor);
    }

    return data_strings;
}


/*
 *  monitor_free()
 *
 *  Frees information returned by monitor_collect.
 */
void monitor_free(char **collected_data)
{
    if (collected_data) {
        while (*collected_data) {
            free(*collected_data++);
        }        
    }
}
