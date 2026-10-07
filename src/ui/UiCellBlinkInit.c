// bdc 0x0892c0f0 UiCellBlinkInit
#include "bdc.h"

/* Initialises a 16-byte cell-blink record `rec`: `{on, phase 0, t 0, sprite}`. */

void UiCellBlinkInit(u8 on, GfxSprite *sprite, UiCellBlink *rec)

{
  memset(rec,0,0x10);
  rec->on = on;
  rec->sprite = sprite;
  return;
}

