// bdc 0x089eb218 UiTextBoxGetPrinter
#include "bdc.h"

/* Returns the text printer of a `UiTextBox` (set by `UiTextBoxCreatePrinter`; NULL until the
   font is ready). */
void *UiTextBoxGetPrinter(void *box)
{
    return ((UiTextBox *)box)->printer;
}
