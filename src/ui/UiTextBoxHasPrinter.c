// bdc 0x089eb104 UiTextBoxHasPrinter
#include "bdc.h"

/* Returns whether the text box already has its text printer (`box+0xc != NULL`), i.e.
   `UiTextBoxCreatePrinter` has succeeded. */

bool UiTextBoxHasPrinter(UiTextBox *box)

{
  return box->printer != (UiTextPrinter *)0x0;
}

