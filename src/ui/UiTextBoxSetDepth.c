// bdc 0x089eb1f0 UiTextBoxSetDepth
#include "bdc.h"

/* Sets a text box's draw depth `+4` and propagates it to its printer (`printer+0x94`). */

void UiTextBoxSetDepth(float depth, UiTextBox *box)

{
  box->depth = depth;
  if (box->printer != (UiTextPrinter *)0x0) {
    box->printer->advanceX = box->depth;
  }
  return;
}

