//#############################################################################
//
//  This file is part of ImagePlay.
//
//  ImagePlay is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ImagePlay is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ImagePlay.  If not, see <http://www.gnu.org/licenses/>.
//
//#############################################################################

#ifndef IPL_GLOBAL_H
#define IPL_GLOBAL_H

#define NOMINMAX

//#include <QtCore/qglobal.h>

#include <algorithm>
#include <math.h>

typedef unsigned char   uchar;
typedef unsigned int    uint;
typedef float           ipl_basetype;

#define FACTOR_TO_FLOAT (1.0f / 255.0f)
#define FACTOR_TO_UCHAR 255.0f

#define PI 3.14159265359


enum IPLDataType
{
    IPL_IMAGE_BW = 0,
    IPL_IMAGE_GRAYSCALE,
    IPL_IMAGE_COLOR,
    IPL_IMAGE_ORIENTED,
    IPL_IMAGE_COMPLEX,
    IPL_POINT,
    IPL_MATRIX,
    IPL_SHAPES,
    IPL_UNDEFINED,

    //Add additional data types here
    //...

    IPL_NUM_DATATYPES
};
//IPLSHARED_EXPORT const char *dataTypeName(IPLDataType type);
const char *dataTypeName(IPLDataType type);

enum IPLProcessWidgetType
{
    IPL_WIDGET_DEFAULT = 0,
    IPL_WIDGET_HIDDEN,

    //Bitfield or Booleans
    IPL_WIDGET_CHECKBOXES,

    //Enumerated integer
    IPL_WIDGET_RADIOBUTTONS,
    IPL_WIDGET_COMBOBOX,

    //Numeric
    IPL_WIDGET_SLIDER,
    IPL_WIDGET_SPINNER,

    //Integer
    IPL_WIDGET_SLIDER_ODD,
    IPL_WIDGET_SLIDER_EVEN,

    //String
    IPL_WIDGET_TEXTFIELD,
    IPL_WIDGET_LABEL,
    IPL_WIDGET_TITLE,
    IPL_WIDGET_FILE_OPEN,
    IPL_WIDGET_FILE_SAVE,
    IPL_WIDGET_FOLDER,

    //Kernel
    IPL_WIDGET_KERNEL,

    //Morphology
    IPL_WIDGET_BINARY_MORPHOLOGY,
    IPL_WIDGET_BINARY_MORPHOLOGY_TRISTATE,
    IPL_WIDGET_GRAYSCALE_MORPHOLOGY,

    //Color
    IPL_WIDGET_COLOR_RGB,
    IPL_WIDGET_COLOR_HSL,
    IPL_WIDGET_COLOR_HSV,

    //Points
    IPL_WIDGET_POINT,

    //Integer: A button that increments the property after each click
    IPL_WIDGET_BUTTON,

    IPL_WIDGET_GROUP,

    IPL_WIDGET_MATRIX,

    //Add additional widget types here
    //...

    IPL_NUM_WIDGETS
};
//IPLSHARED_EXPORT const char *widgetName(IPLProcessWidgetType type);
const char *widgetName(IPLProcessWidgetType type);

enum IPLRawImageType
{
    IPL_RAW_8BIT = 0,
    IPL_RAW_24BIT_RGB,
    IPL_RAW_24BIT_BGR,
    IPL_RAW_32BIT_RGB,
    IPL_RAW_32BIT_BGR
};

#endif // IPL_GLOBAL_H
