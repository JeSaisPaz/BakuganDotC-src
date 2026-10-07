// bdc 0x089eaf60 UiTextBoxCtor
#include "bdc.h"

/* Constructor of the 0x10-byte text box helper used by full-screen notice tasks (copyright,
   autosave): clears the depth/position floats `+0x0`/`+0x4`, the needs-clear byte `+0x8` and the
   text printer pointer `+0xc`. Returns `box`. */

UiTextBox *UiTextBoxCtor(UiTextBox *box)

{
  box->printer = (UiTextPrinter *)0x0;
  box->depth = 0.0;
  box->drawn = '\0';
  box->packetDepth = 0.0;
  return box;
}

