// bdc 0x089317f4 UiGauntletSetupResetState
#include "bdc.h"

/* Clears the state of `UiGauntletSetup` (task 373) at construction: focus
   area `+0x74` = 0, item `+0x76` = 0, `+0x77` = 0xff, the three model pointers (`+0x1a7c`,
   `+0x1a80`, `+0x1a84`, `+0x1af0`) and the flags `+0x1b5c/+0x1b5d`; loads the card slots
   (`UiGauntletSetupLoadCardSlots`) and zeroes the text-box block `+0xcb0` (0x448 bytes) and
   `+0x1a68` (0x10). */

void UiGauntletSetupResetState(UiGauntletSetup *self)

{
  self->item = '\0';
  self->prevItem = -1;
  self->focusArea = '\0';
  self->views = (void *)0x0;
  self->bakuganModel = (void *)0x0;
  self->pedestalModel = (void *)0x0;
  self->playerModel = (void *)0x0;
  self->pushRequested = '\0';
  self->pushPlaying = '\0';
  UiGauntletSetupLoadCardSlots(self);
  memset(&self->namePrinter,0,0x448);
  memset(self->cardInfo,0,0x10);
  return;
}

