// bdc 0x089eba34 UiTextTaskDraw
#include "bdc.h"

/* Draw method of the text task (`UiTextTask`) (vtable `0x08af56e4` slot 4): draws the message box
   frame (`UiMsgBoxDraw`) and the message window (`UiMsgWindowDraw`) when they exist, then, as
   far as the init `step` has progressed, the shared text box (step >= 1), the message box text
   (step >= 2, box with owner) and the visible message window text (step >= 3) with
   `UiTextBoxDraw`. */

void UiTextTaskDraw(CoreTask *task)
{
  UiTextTask *self = (UiTextTask *)task;
  UiMsgBox *msgBox;
  UiMsgWindow *msgWindow;

  if (UiMsgBoxExists()) {
    UiMsgBoxDraw(UiMsgBoxGet());
  }
  if (UiMsgWindowExists()) {
    UiMsgWindowDraw(UiMsgWindowGet());
  }
  if (self->step > 0) {
    UiTextBoxDraw(UiTextRenderGetBox());
  }
  if (self->step >= 2 && UiMsgBoxExists()) {
    msgBox = UiMsgBoxGet();
    if (UiMsgBoxHasOwner(msgBox)) {
      msgBox = UiMsgBoxGet();
      UiTextBoxDraw(msgBox->textBox);
    }
  }
  if (self->step >= 3 && UiMsgWindowExists()) {
    msgWindow = UiMsgWindowGet();
    if (UiMsgWindowIsVisible(msgWindow)) {
      msgWindow = UiMsgWindowGet();
      UiTextBoxDraw(msgWindow->textBox);
    }
  }
}
