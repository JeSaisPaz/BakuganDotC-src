// bdc 0x089eb720 UiHasTextTask
#include "bdc.h"

/* Returns whether `g_uiTextTask` is non-null (singleton accessor, named by `bdc singleton`). */

bool UiHasTextTask(void)

{
  return g_uiTextTask != (CoreTask *)0x0;
}

