// bdc 0x089ac064 UiPauseSettingsPlayPreview
#include "bdc.h"

/* Plays the feedback sound after a value change: for the voice slider (item 1) a voice sample
   (`0x430001e`, stopping the previous one kept in `previewVoice`), for BGM, SE and the toggle the
   cursor sound 1 (`SndManagerPlay`). Cursor values 4 and up and negative play nothing. */

void UiPauseSettingsPlayPreview(UiPauseSettings *self)
{
  s8 cursor = self->cursor;

  if (cursor < 0) {
    return;
  }
  if (cursor < 2) {
    if (cursor > 0) {
      if (self->previewVoice != 0 && SndHasManager()) {
        SndManagerStop(SndGetManager(), self->previewVoice);
      }
      self->previewVoice = SndManagerPlay(SndGetManager(), 0x430001e, 0, 0);
      return;
    }
  } else if (cursor >= 4) {
    return;
  }
  if (SndHasManager()) {
    SndManagerPlay(SndGetManager(), 1, 0, 0);
  }
}
