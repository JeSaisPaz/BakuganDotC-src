// bdc 0x089eb20c UiTextBoxGetColor
#include "bdc.h"

/* Returns a pointer to the text printer's RGBA colour (four floats at printer `+0xc0`); the
   copyright task copies a colour constant into it before printing. */

float *UiTextBoxGetColor(UiTextBox *box)

{
  return box->printer->outlineColor;
}

