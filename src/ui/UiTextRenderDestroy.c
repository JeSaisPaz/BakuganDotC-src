// bdc 0x089eab50 UiTextRenderDestroy
#include "bdc.h"

/* Destroys the shared text renderer: removes the text task (id 0x2742) if present
   (`CoreTaskRemoveById`), deletes its text box (`UiTextBoxDelete`) and frees the holder
   `g_uiTextRenderHolder`. Called by `UiTextTaskDtor`. */

void UiTextRenderDestroy(void)

{
  if (CoreTaskFind(0x2742) != NULL) {
    CoreTaskRemoveById(0x2742);
  }
  if (g_uiTextRenderHolder != NULL) {
    if (*g_uiTextRenderHolder != NULL) {
      UiTextBoxDelete((UiTextBox *)*g_uiTextRenderHolder, 3);
      *g_uiTextRenderHolder = NULL;
    }
    if (g_uiTextRenderHolder != NULL) {
      MemLock();
      MemFree(g_uiTextRenderHolder, NULL, 0);
      MemUnlock();
      g_uiTextRenderHolder = NULL;
    }
  }
}
