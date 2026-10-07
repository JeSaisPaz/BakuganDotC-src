// bdc 0x089ec3b0 UiMsgBoxIsFinished
#include "bdc.h"

/* Returns `box+0x0c > 3` (field `id`, used as the box state by UiMsgBoxUpdate), i.e. the box has finished closing. */

bool UiMsgBoxIsFinished(UiMsgBox *self)

{
  return 3 < self->id;
}

