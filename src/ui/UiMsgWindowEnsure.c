// bdc 0x08816828 UiMsgWindowEnsure
#include "bdc.h"

/* Creates the message window singleton `g_uiMsgWindow` if it does not exist (0x48 bytes,
   `UiMsgWindowCtor`) and makes sure the text renderer task exists (`UiTextRenderExists` /
   `UiTextRenderEnsure`). Used by the save tasks' constructors before they show messages. */

void UiMsgWindowEnsure(void)
{
  UiMsgWindow *window;
  bool fromLow;
  UiMsgWindow *self;

  window = g_uiMsgWindow;
  if (g_uiMsgWindow == (void *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    self = MemAlloc(0x48, (char *)0x0, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    window = (void *)0x0;
    if (self != (UiMsgWindow *)0x0) {
      UiMsgWindowCtor(self);
      window = self;
    }
  }
  g_uiMsgWindow = window;
  if (!UiTextRenderExists()) {
    UiTextRenderEnsure();
  }
  return;
}
