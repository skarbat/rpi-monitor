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

#define _GNU_SOURCE

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termio.h>
#include <unistd.h>
#include <sys/time.h>

#include "bcm_host.h"

#include "backgroundLayer.h"
#include "font.h"
#include "imageLayer.h"
#include "key.h"

#include "interface/vmcs_host/vcgencmd.h"

#include "version.h"

#include "gui.h"
#include "monitor.h"


int verbose=0;
void vprint(char *literal)
{
    if (verbose) {
        puts(literal);
    }
}

int parse_color(char *literal, PGUI_COLOR pcolor_dest)
{
    int coloured=0;

    if (!pcolor_dest) {
        return -1;
    }

    char *token=strtok(literal, ",");
    if(token) {
        coloured++;
        pcolor_dest->r=atoi(token);

        token=strtok(NULL, ",");
        if(token) {
            coloured++;
            pcolor_dest->g=atoi(token);

            token=strtok(NULL, ",");
            if(token) {
                coloured++;
                pcolor_dest->b=atoi(token);
            }
        }
    }

    return coloured;
}

void display_help(void)
{
    printf ("  -d   display number for multiple screens\n");
    printf ("  -x   window X position\n");
    printf ("  -y   window Y position\n");
    printf ("  -w   window Width\n");
    printf ("  -h   window Height\n");
    printf ("  -l   layer number\n");
    printf ("  -u   update frequency in seconds\n");
    printf ("  -c   data collector process pathname\n");
    printf ("  -o   window opacity level (0-255)\n");
    printf ("  -f   text color (r,g,b)\n");
    printf ("  -b   window color (r,g,b)\n");
    printf ("  -v   be verbose\n");
    printf ("  -?   this help scren\n\n");
}


int main(int argc, char *argv[])
{
    int opt = 0;
    int8_t opacity = 200;
    uint32_t displayNumber = 0, layer = 32000;
    GUI_COLOR color_text={0,0,0}, color_win={255,255,255};
    char *collector_process="/usr/share/rpi-monitor/collectors/rpi-monitor-gpumem.py";
    int x_position=10, y_position=10, width=250, height=175;
    int wait_seconds=1;

    printf ("rpi-monitor version %s\n\n", RPI_MONITOR_VERSION);

    while ((opt = getopt(argc, argv, "d:x:y:w:h:l:u:c:o:f:b:v?")) != -1)
    {
        switch (opt)
            {
            case 'd':
                displayNumber = atoi(optarg);
                break;
            case 'x':
                x_position = atoi(optarg);
                break;
            case 'y':
                y_position = atoi(optarg);
                break;
            case 'w':
                width = atoi(optarg);
                break;
            case 'h':
                height = atoi(optarg);
                break;
            case 'l':
                layer = atoi(optarg);
                break;
            case 'u':
                wait_seconds = atoi(optarg);
                break;
            case 'c':
                collector_process = strdup(optarg);
                break;
            case 'o':
                opacity = atoi(optarg);
                break;
            case 'f':
                if (parse_color(optarg, &color_text) != 3) {
                    printf ("Invalid text colour specified\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'b':
                if (parse_color(optarg, &color_win) != 3) {
                    printf ("Invalid window colour specified\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'v':
                verbose=1;
                break;

            default:
                printf ("unrecognized option\n");
            case '?':
                display_help();
                exit(EXIT_FAILURE);
            }
    }


    vprint("Initializing video GUI resources");
    PGUI_PARAMS pgui = gui_init(displayNumber, x_position, y_position, width, height, layer, opacity, &color_win, &color_text);
    GUI_DATA gui_data;

    while (true) {
        vprint("Collecting data from external process");
        gui_data.info_lines = monitor_collect(collector_process);

        vprint("Rendering information on the screen");
        gui_render(pgui, &gui_data);

        vprint("Deallocate data and delaying");
        monitor_free(gui_data.info_lines);        
        sleep(wait_seconds);
    }

    vprint("Releasing resources and terminating");
    gui_terminate(pgui);
    exit(EXIT_SUCCESS);
}
