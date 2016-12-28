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

#include "gui.h"


/*
 *  gui_init()
 *
 *  Initializes access to Dispmanx graphics.
 *  Returns a structure needed to render the information window.
 *  Call gui_terminate() to free the resources.
 */

PGUI_PARAMS gui_init(int displayNumber, int xpos, int ypos, int infoLayerWidth, int infoLayerHeight,
                     int layerNumber, int windowOpacity, PGUI_COLOR color_win, PGUI_COLOR color_text)
{
    int result=-1;

    bcm_host_init();

    // dispmanx access is ok, time to allocate GUI resources
    PGUI_PARAMS pgui_params=calloc(sizeof(GUI_PARAMS), 1);
    if (!pgui_params) {
        return NULL;
    }

    // obtain basic access to dispmanx graphics
    pgui_params->displayHandle = vc_dispmanx_display_open(displayNumber);
    assert(pgui_params->displayHandle != 0);

    // allocate and save GUI resources
    DISPMANX_MODEINFO_T info;
    result = vc_dispmanx_display_get_info(pgui_params->displayHandle, &info);
    assert(result == 0);

    pgui_params->displayNumber=displayNumber;
    pgui_params->xpos=xpos;
    pgui_params->ypos=ypos;
    pgui_params->infoLayerWidth=infoLayerWidth;
    pgui_params->infoLayerHeight=infoLayerHeight;
    pgui_params->layerNumber=layerNumber;
    pgui_params->windowOpacity=windowOpacity;

    pgui_params->backgroundColour.red = color_win->r;
    pgui_params->backgroundColour.green = color_win->g;
    pgui_params->backgroundColour.blue = color_win->b;
    pgui_params->backgroundColour.alpha = pgui_params->windowOpacity;

    pgui_params->textColour.red = color_text->r;
    pgui_params->textColour.green = color_text->g;
    pgui_params->textColour.blue = color_text->b;
    pgui_params->textColour.alpha = pgui_params->windowOpacity;;

    initImageLayer(&(pgui_params->infoLayer), pgui_params->infoLayerWidth, pgui_params->infoLayerHeight, VC_IMAGE_RGBA16);
    createResourceImageLayer(&(pgui_params->infoLayer), pgui_params->layerNumber);

    // initial display of information window on the screen
    DISPMANX_UPDATE_HANDLE_T update = vc_dispmanx_update_start(0);
    assert(update != 0);

    addElementImageLayerOffset(&(pgui_params->infoLayer), pgui_params->xpos, pgui_params->ypos, pgui_params->displayHandle, update);

    pgui_params->image = &(pgui_params->infoLayer.image);
    clearImageRGB(pgui_params->image, &pgui_params->backgroundColour);
    changeSourceAndUpdateImageLayer(&(pgui_params->infoLayer));

    result = vc_dispmanx_update_submit_sync(update);
    assert(result == 0);

    return pgui_params;
}


/*
 *  gui_render()
 *
 *  Renders the information window on the screen.
 *  Data to display is collected from gui_data.
 *  Returns immediately after rendering the window.
 *
 */
int gui_render(PGUI_PARAMS pgui_params, PGUI_DATA pgui_data)
{
    int result=-1;
    int horizontalOffset=10, verticalOffset=10, fontVerticalSize=22;

    // obtain a sequence to refresh the graphic window
    DISPMANX_UPDATE_HANDLE_T update = vc_dispmanx_update_start(0);
    assert(update != 0);
    clearImageRGB(pgui_params->image, &(pgui_params->backgroundColour));

    // collect and render the information literals
    if (pgui_data && pgui_data->info_lines) {
        char **next_string=pgui_data->info_lines;
        while(*next_string) {
            drawStringRGB(horizontalOffset, verticalOffset, *next_string++, &pgui_params->textColour, pgui_params->image);
            verticalOffset += fontVerticalSize;
        }
    }
    else {
        drawStringRGB(horizontalOffset, verticalOffset, NO_DATA_ERROR, &pgui_params->textColour, pgui_params->image);
    }

    // update the graphics on the screen
    changeSourceAndUpdateImageLayer(&pgui_params->infoLayer);
    result = vc_dispmanx_update_submit_sync(update);
    assert(result == 0);

    return true;
}


/*
 *  gui_terminate()
 *
 *  Deallocates GUI resources and releases access to Dispmanx graphics.
 *
 */    
void gui_terminate(PGUI_PARAMS pgui_params)
{
    int result=-1;

    destroyImageLayer(&pgui_params->infoLayer);
    result = vc_dispmanx_display_close(pgui_params->displayHandle);

    // TODO: more stuff?
    
    free (pgui_params);
    assert(result == 0);
}
