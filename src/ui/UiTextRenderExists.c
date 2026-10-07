// bdc 0x089eabd8 UiTextRenderExists
#include "bdc.h"

/* Returns 1 when the shared text-render holder `g_uiTextRenderHolder` and the text box (`UiTextBoxCtor`
   layout) it points to exist. */

bool UiTextRenderExists(void)

{
  bool exists;

  exists = false;
  if ((g_uiTextRenderHolder != (void **)0x0) && (*g_uiTextRenderHolder != (void *)0x0)) {
    exists = true;
  }
  return exists;
}
