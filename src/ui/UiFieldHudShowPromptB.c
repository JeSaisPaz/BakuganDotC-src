// bdc 0x088d16c8 UiFieldHudShowPromptB
#include "bdc.h"

/* Variant of `UiFieldHudShowPromptA` for the field HUD (`UiFieldHudCtor`): the prompt slides
   in while either player flag `promptA` or `idleHint` is set and slides out (then is hidden, its
   glyphs cleared, `promptY` reset to -48) once both are clear. Same measurement (`UiTextMeasure`,
   short/long background sprites 0x2d/0x2e) and black-then-green shadowed print (printer vtable
   slot 2), except that while the player's `scan` flag is set the background is hidden and the
   printer's glyphs are cleared instead of printed. Only `promptA` is auto-cleared after
   `g_uiFieldHudPromptParamB` frames (`promptHold`). */

void UiFieldHudShowPromptB(UiFieldHud *self)
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
  measured = UiTextMeasure(0.0f, self->hintPrinter, (char *)PspPtr(self->hints[(u16)self->hintId]), NULL, NULL,
                           NULL);
  if (measured < 0x10) {
    sprite = 0x2d;
    textY = 7.0f;
  }
  else {
    sprite = 0x2e;
  }

  if (self->promptShown == 0) {
    if (self->player->promptA != 0 || self->player->idleHint != 0) {
      sprites = (GfxSprite **)self->base.data;
      self->promptShown = 1;
      self->promptVisible = 1;
      sprites[sprite]->flags |= 1;
    }
  }
  else if (self->player->promptA == 0 && self->player->idleHint == 0) {
    self->promptVisible = 1;
  }
  if (self->promptShown == 0) {
    return;
  }

  if (self->player->scan != 0) {
    ((GfxSprite **)self->base.data)[sprite]->flags &= ~1u;
    if (self->hintPrinter->glyphs != NULL) {
      printer = self->hintPrinter;
      GfxSpriteLayerClear(&printer->layer);
      printer->glyphs = NULL;
    }
  }
  else {
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
                           (char *)PspPtr(self->hints[(u16)self->hintId]), 1, 0, 0);
      printer = self->hintPrinter;
      printer->outlineColor[0] = g_colorGreen.x;
      printer->outlineColor[1] = g_colorGreen.y;
      printer->outlineColor[2] = g_colorGreen.z;
      printer->outlineColor[3] = g_colorGreen.w;
      printer = self->hintPrinter;
      entry = &printer->layer.vtbl[2];
      ((PrintFn)entry->fn)(240.0f, textY, 0.0f, (u8 *)printer + entry->delta,
                           (char *)PspPtr(self->hints[(u16)self->hintId]), 1, 0, 0);
    }
  }

  if (self->promptVisible != 0) {
    self->promptTimer = self->promptTimer + 1;
    y = self->promptY;
    ((GfxSprite **)self->base.data)[sprite]->posY = y;
    self->hintPrinter->layer.view.w.y = y;
    if (self->player->promptA != 0 || self->player->idleHint != 0) {
      self->promptY = self->promptY + g_uiFieldHudPromptScale;
      if (g_uiFieldHudPromptParamA < self->promptTimer) {
        self->promptTimer = 0;
        self->promptY = 48.0f;
        self->promptVisible = 0;
      }
    }
    else if (self->player->promptA == 0 && self->player->idleHint == 0) {
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
