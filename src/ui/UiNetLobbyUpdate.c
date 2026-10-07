// bdc 0x089418cc UiNetLobbyUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the ad-hoc multiplayer lobby screen (task id 2000): first
   steps the text box setup on `printerStep` (0: `UiTextBoxCreatePrinter` with 0x80 chars, 1:
   `UiTextPrinterSetFont` font 1, each advancing the step once it succeeds; 2: clears the text
   box every frame if there is one), then runs the phase handler selected by `phase` (`+0x28`)
   from the 5-entry pointer-to-member table `g_uiNetLobbyPhaseTable`, then
   `UiScreenUpdateCommon`. */

void UiNetLobbyUpdate(UiNetLobby *self)
{
  s32 step = self->printerStep;
  s32 phase;

  if (step == 0) {
    if (UiTextBoxCreatePrinter(self->textBox, 0x80) != 0) {
      self->printerStep++;
    }
  } else if (step == 1) {
    if (UiTextPrinterSetFont(UiTextBoxGetPrinter(self->textBox), 1) != 0) {
      self->printerStep++;
    }
  } else if (step == 2) {
    if (self->textBox != NULL) {
      UiTextBoxClear(self->textBox);
    }
  }

  phase = self->base.phase;
  if (phase >= 0 && phase < 5) {
    const MemberFnPtr *member = &g_uiNetLobbyPhaseTable[phase];
    u8 *obj = (u8 *)self + member->delta;
    void *fn = member->pfn;

    if (member->index != 0) {
      const VtblEntry *entry =
          &(*(const VtblEntry **)(obj + (uintptr_t)member->pfn))[member->index];

      fn = entry->fn;
      obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
  }
  UiScreenUpdateCommon(&self->base);
}
