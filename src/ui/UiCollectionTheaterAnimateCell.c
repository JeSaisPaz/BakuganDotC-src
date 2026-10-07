// bdc 0x0898995c UiCollectionTheaterAnimateCell
#include "bdc.h"

/* Animates (`UiCursorGlowStep`) the selected scene cell sprite of
   `UiCollectionTheater`. */

void UiCollectionTheaterAnimateCell(UiScreen *screen)

{
  UiCursorGlowStep(((GfxSprite **)screen->data)[((UiCollectionTheater *)screen)->cursor]);
  return;
}
