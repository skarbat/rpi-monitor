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

// Message displayed if external collector program fails
#define NO_DATA_ERROR "No data collected"

typedef struct _GUI_PARAMS {
    int displayNumber;
    int xpos;
    int ypos;
    int infoLayerWidth;
    int infoLayerHeight;
    int layerNumber;
    int windowOpacity;

    DISPMANX_DISPLAY_HANDLE_T displayHandle;
    IMAGE_LAYER_T infoLayer;
    IMAGE_T *image;
    RGBA8_T backgroundColour;
    RGBA8_T textColour;

} GUI_PARAMS, *PGUI_PARAMS;

typedef struct _GUI_DATA {
    char **info_lines;

} GUI_DATA, *PGUI_DATA;

typedef struct _GUI_COLOR {
    uint8_t r, g, b;

} GUI_COLOR, *PGUI_COLOR;

PGUI_PARAMS gui_init(int displayNumber, int xpos, int ypos, int infoLayerWidth, int infoLayerHeight,
                     int layerNumber, int windowOpacity, PGUI_COLOR color_win, PGUI_COLOR color_text);

int gui_render(PGUI_PARAMS pgui_params, PGUI_DATA pgui_data);
void gui_terminate(PGUI_PARAMS pgui_params);
