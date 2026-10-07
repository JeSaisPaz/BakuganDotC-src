// bdc 0x089eac00 UiTextRenderGetBox
#include "bdc.h"

/* Returns the shared text box held by `g_uiTextRenderHolder` (no NULL check; see `UiTextRenderExists`). */

void *UiTextRenderGetBox(void)

{
  return *g_uiTextRenderHolder;
}

