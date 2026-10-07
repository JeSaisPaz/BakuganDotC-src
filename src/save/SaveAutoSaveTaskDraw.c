// bdc 0x0880baa8 SaveAutoSaveTaskDraw
#include "bdc.h"

/* Draw method of the autosave task (id 10022, vtable slot 4): draws
   its private fader (`GfxFaderDrawStep`) when present and its text box (`UiTextBoxDraw`). */

void SaveAutoSaveTaskDraw(CoreTask *task)

{
  SaveAutoSaveTask *self = (SaveAutoSaveTask *)task;

  if (self->fader != NULL) {
    GfxFaderDrawStep(self->fader);
  }
  if (self->textBox != NULL) {
    UiTextBoxDraw(self->textBox);
  }
}
