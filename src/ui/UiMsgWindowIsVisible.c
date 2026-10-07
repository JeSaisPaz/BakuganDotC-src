// bdc 0x08816e4c UiMsgWindowIsVisible
#include "bdc.h"

/* Returns whether the message window (`UiMsgWindowCtor`) is visible (alpha `+0x8` ≠ 0). Used by
   the text render task (`UiTextTaskDraw`) to decide whether to draw it. */

bool UiMsgWindowIsVisible(UiMsgWindow *self)

{
  return self->alpha != 0.0f;
}

