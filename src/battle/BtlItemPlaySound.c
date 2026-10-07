// bdc 0x088b6844 BtlItemPlaySound
#include "bdc.h"

/* Plays the positional sound `soundId` for a battle item at `pos`, or at the item's own position
   `pos` (`+0x20`) when `pos` is NULL, as a one-shot auto-freed emitter (`SndEmitterCreateAtPos`
   with loop 0, autoFree 1; no `SndHasListener` check). */

void BtlItemPlaySound(BtlItem *item, s32 soundId, float *pos)
{
  if (pos == NULL) {
    SndEmitterCreateAtPos(SndGetListener(), soundId, item->pos, 0, 1);
  } else {
    SndEmitterCreateAtPos(SndGetListener(), soundId, pos, 0, 1);
  }
}
