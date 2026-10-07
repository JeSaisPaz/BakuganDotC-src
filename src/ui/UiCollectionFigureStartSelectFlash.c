// bdc 0x0898e3bc UiCollectionFigureStartSelectFlash
#include "bdc.h"

/* Starts the 4-frame add-colour flash (`UiFlashStart`, flash slot 0) on the selected cell sprite
   (data `[cursor +0xe78]`) of `UiCollectionFigure` when an entry is
   opened; `UiCollectionFigureSelectFlashDone` waits for it. */

void UiCollectionFigureStartSelectFlash(UiCollectionFigure *self)
{
  UiFlashStart(4.0f, ((GfxSprite **)self->base.data)[self->cursor], 0, 0);
}
