// bdc 0x089ebb3c UiMsgBoxEnsure
#include "bdc.h"

/* Lazily creates the message box singleton behind `g_uiMsgBoxHolder` (a 4-byte holder in the low
   heap pointing to a 0x80-byte box built by `UiMsgBoxCtor`) and makes sure the shared text
   renderer exists (`UiTextRenderExists`/`UiTextRenderEnsure`). Returns the box
   `*g_uiMsgBoxHolder` (NULL if its allocation failed). */

void *UiMsgBoxEnsure(void)
{
  bool fromLow;
  void **holder;
  UiMsgBox *alloc;
  UiMsgBox *box;

  if (g_uiMsgBoxHolder == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    holder = MemAlloc(sizeof(void *), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    g_uiMsgBoxHolder = holder;
    memset(holder, 0, sizeof(void *));
    box = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    alloc = MemAlloc(sizeof(UiMsgBox), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (alloc != NULL) {
      UiMsgBoxCtor(alloc);
      box = alloc;
    }
    *g_uiMsgBoxHolder = box;
  }
  if (!UiTextRenderExists()) {
    UiTextRenderEnsure();
  }
  return *g_uiMsgBoxHolder;
}
