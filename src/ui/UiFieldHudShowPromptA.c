// bdc 0x088d061c UiFieldHudShowPromptA
#include "bdc.h"

/* Slide-in text prompt of the field HUD (`UiFieldHudCtor`) driven by the player flag
   `promptA`: measures `hints[hintId]` with `hintPrinter` (`UiTextMeasure`) and picks the short
   (sprite 0x2d, text 7 px lower; return value below 16) or long (0x2e) background. When the
   flag rises, the prompt is shown and starts sliding (`promptVisible` = animating): each frame the
   background and printer Y follow `promptY`, which grows by `g_uiFieldHudPromptScale` until
   `promptTimer` passes `g_uiFieldHudPromptParamA` (then parked at 48). If the printer has no glyphs
   the text is printed twice (black at x 241 / y+1, then green at x 240: a drop shadow, printer
   vtable slot 2). When the flag drops, it slides back the same way and is then hidden, its glyphs
   cleared, `promptY` reset to -48 and `promptShown` cleared. While the flag is set `promptHold`
   counts frames and clears the flag after `g_uiFieldHudPromptParamB`. */

void UiFieldHudShowPromptA(UiFieldHud *self)
{
  typedef void (*PrintFn)(float, float, float, void *, char *, s32, s32, s32);
  GfxSprite **sprites;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  float textY;
  float y;
  int sprite;
  int measured;

  textY = 0.0f;
  measured = UiTextMeasure(0.0f, self->hintPrinter, self->hints[(u16)self->hintId], NULL, NULL,
                           NULL);
  if (measured < 0x10) {
    sprite = 0x2d;
    textY = 7.0f;
  }
  else {
    sprite = 0x2e;
  }

  if (self->promptShown == 0) {
    if (self->player->promptA != 0) {
      sprites = (GfxSprite **)self->base.data;
      self->promptShown = 1;
      self->promptVisible = 1;
      sprites[sprite]->flags |= 1;
    }
  }
  else if (self->player->promptA == 0) {
    self->promptVisible = 1;
  }
  if (self->promptShown == 0) {
    return;
  }

  ((GfxSprite **)self->base.data)[sprite]->flags |= 1;
  if (self->hintPrinter->glyphs == NULL) {
    printer = self->hintPrinter;
    printer->outlineColor[0] = g_colorBlack.x;
    printer->outlineColor[1] = g_colorBlack.y;
    printer->outlineColor[2] = g_colorBlack.z;
    printer->outlineColor[3] = g_colorBlack.w;
    printer = self->hintPrinter;
    entry = &printer->layer.vtbl[2];
    ((PrintFn)entry->fn)(241.0f, textY + 1.0f, 0.0f, (u8 *)printer + entry->delta,
                         self->hints[(u16)self->hintId], 1, 0, 0);
    printer = self->hintPrinter;
    printer->outlineColor[0] = g_colorGreen.x;
    printer->outlineColor[1] = g_colorGreen.y;
    printer->outlineColor[2] = g_colorGreen.z;
    printer->outlineColor[3] = g_colorGreen.w;
    printer = self->hintPrinter;
    entry = &printer->layer.vtbl[2];
    ((PrintFn)entry->fn)(240.0f, textY, 0.0f, (u8 *)printer + entry->delta,
                         self->hints[(u16)self->hintId], 1, 0, 0);
  }

  if (self->promptVisible != 0) {
    self->promptTimer = self->promptTimer + 1;
    y = self->promptY;
    ((GfxSprite **)self->base.data)[sprite]->posY = y;
    self->hintPrinter->layer.view.w.y = y;
    if (self->player->promptA != 0) {
      self->promptY = self->promptY + g_uiFieldHudPromptScale;
      if (g_uiFieldHudPromptParamA < self->promptTimer) {
        self->promptTimer = 0;
        self->promptY = 48.0f;
        self->promptVisible = 0;
      }
    }
    else if (self->player->promptA == 0) {
      self->promptY = self->promptY - g_uiFieldHudPromptScale;
      if (g_uiFieldHudPromptParamA < self->promptTimer) {
        ((GfxSprite **)self->base.data)[sprite]->flags &= ~1u;
        if (self->hintPrinter->glyphs != NULL) {
          printer = self->hintPrinter;
          GfxSpriteLayerClear(&printer->layer);
          printer->glyphs = NULL;
        }
        self->promptTimer = 0;
        self->promptY = -48.0f;
        self->promptVisible = 0;
        self->promptShown = 0;
      }
    }
  }

  if (self->player->promptA != 0) {
    self->promptHold = self->promptHold + 1;
    if (g_uiFieldHudPromptParamB < self->promptHold) {
      self->promptHold = 0;
      self->player->promptA = 0;
    }
  }
}
