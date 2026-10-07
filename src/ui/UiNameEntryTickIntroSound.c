// bdc 0x08805ec4 UiNameEntryTickIntroSound
#include "bdc.h"

/* Intro-animation helper of `UiNameEntry`: increments `introSoundTimer` and,
   once it reaches 7, plays sound `0x2c00001` (if a sound manager exists) and resets it to 0.
   Called each frame by `UiNameEntryIntroPhase`. */

void UiNameEntryTickIntroSound(UiNameEntry *self)
{
  self->introSoundTimer++;
  if (self->introSoundTimer >= 7) {
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x2c00001, 0, 0);
    }
    self->introSoundTimer = 0;
  }
}
