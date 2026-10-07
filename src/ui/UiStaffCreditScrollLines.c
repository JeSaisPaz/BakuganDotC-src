// bdc 0x08945b98 UiStaffCreditScrollLines
#include "bdc.h"

/* Scrolls the credits of `UiStaffCredit` one frame: spawns due lines
   (`UiStaffCreditSpawnLine`), moves every active line printer (`glyphs` set) up by 1 px and clears
   it (`GfxSpriteLayerClear`, `glyphs` = NULL) once it has left the top (Y ≤ −24·scale); then, for
   the spawned lines from `unk84` up to `lineIndex` (`g_staffCreditLayout` `lines`), scrolls the
   sprite of each non-text line (index `g_staffCreditKindSprite` by kind, kind 8 → sprite 0x1e,
   kind 9 = text, skipped) up by 1 px, or hides it (`flags` bit 0) and advances `unk84` once it is
   fully above the screen (`GfxSpriteGetHeight`). */

void UiStaffCreditScrollLines(UiScreen *screen)
{
  UiStaffCredit *credit = (UiStaffCredit *)screen;
  int i;

  UiStaffCreditSpawnLine(screen);
  for (i = 0; i < 20; i++) {
    UiTextPrinter *printer = (UiTextPrinter *)credit->overlays[i];
    if (printer->glyphs != NULL) {
      if (printer->layer.view.w.y <= printer->scale * -24.0f) {
        GfxSpriteLayerClear(&printer->layer);
        printer->glyphs = NULL;
      } else {
        printer->layer.view.w.y = printer->layer.view.w.y - 1.0f;
      }
    }
  }

  for (i = (int)credit->unk84; i < credit->lineIndex; i++) {
    UiStaffCreditLine *line = &g_staffCreditLayout.lines[i];
    if (line->kind != 9) {
      GfxSprite *sprite;
      float y;
      float h;
      int idx = g_staffCreditKindSprite[line->kind];
      if (line->kind == 8) {
        idx = 0x1e;
      }
      sprite = ((GfxSprite **)screen->data)[idx];
      y = sprite->posY;
      h = GfxSpriteGetHeight(sprite);
      sprite = ((GfxSprite **)screen->data)[idx];
      if (y <= -h) {
        sprite->flags &= ~1u;
        credit->unk84++;
      } else {
        sprite->posY = sprite->posY - 1.0f;
      }
    }
  }
}
