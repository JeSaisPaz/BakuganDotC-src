// bdc 0x089eb68c UiTextClearAll
#include "bdc.h"

/* Per-frame text reset run from `CoreTaskManagerUpdate`: unless the text task `g_uiTextTask`
   exists and is not allowed to run, clears the shared text box, the message box text and the
   `g_uiMsgWindow` text (`UiTextBoxClear`). */

void UiTextClearAll(void)

{
  if (g_uiTextTask == NULL || CoreTaskIsIdAllowed(g_uiTextTask->id)) {
    if (UiTextRenderExists()) {
      UiTextBoxClear((UiTextBox *)UiTextRenderGetBox());
    }
    if (UiMsgBoxExists()) {
      UiTextBoxClear((UiTextBox *)((UiMsgBox *)UiMsgBoxGet())->textBox);
    }
    if (UiMsgWindowExists()) {
      UiTextBoxClear((UiTextBox *)((UiMsgWindow *)UiMsgWindowGet())->textBox);
    }
  }
}
