// bdc 0x0890d9e0 UiLoadingStateSetup
#include "bdc.h"

/* State 1 handler of the now-loading screen (task 10100, `UiLoadingCtor`), driven by the sub-step
   `step`. Step 0: once the shared message box (`g_uiLoadingShared``->box`) has its printer, sets
   font 0, spacing 3 and wrap width 460, then advances. Step 1: builds the screen from the 23 entries
   of UI layout 0x2b (icons 11..16 sized 80x80 with their cell from `g_loadingIconLayout`, the
   others positioned and UV-mapped), adjusts sprites 21/22, shows or hides them by theme and the
   icons by the ball texture, selects the language cell of sprites 9/22 (`g_loadingLangCells`),
   creates the flash fab once, then advances. Any other step: back to step 1 while there is no fab;
   otherwise prints the tip text (y by its measured width), rewinds the fab, resets the tip scroll
   and advances `state`. Always ends with `UiLoadingSyncTipScroll` once the box has a printer. */

void UiLoadingStateSetup(UiLoading *self)
{
  UiLoadingShared *shared;
  UiTextPrinter *printer;
  UiLayoutEntry *entry;
  GfxSprite *sprite;
  GfxFab *fab;
  void *mem;
  char *text;
  float width;
  float col;
  float row;
  bool fromLow;
  s32 lang;
  s32 i;

  if (self->step == 0) {
    if (UiTextBoxHasPrinter(g_uiLoadingShared->box)) {
      printer = (UiTextPrinter *)UiTextBoxGetPrinter(g_uiLoadingShared->box);
      UiTextPrinterSetFont(printer, 0);
      printer = (UiTextPrinter *)UiTextBoxGetPrinter(g_uiLoadingShared->box);
      printer->spacing = 3.0f;
      printer = (UiTextPrinter *)UiTextBoxGetPrinter(g_uiLoadingShared->box);
      printer->wrapWidth = 460.0f;
      self->step = self->step + 1;
    }
  } else if (self->step == 1) {
    for (i = 0; i < 23; i++) {
      entry = (UiLayoutEntry *)UiLayoutGetEntry(0x2b, i);
      sprite = g_uiLoadingShared->sprites[i];
      sprite->texture = GfxFindTexture(entry->texture);
      if (i >= 11 && i <= 16) {
        UiSpriteSetSize(80.0f, 80.0f, sprite);
        col = 0.0f;
        row = 0.0f;
        UiLoadingIconCell(self,
                          ((u8 *)&g_loadingIconLayout[i - 11][8])[(s32)sprite->maybe_billboardParams80[3]],
                          &col, &row);
        GfxSpriteSetCell(sprite, col, row);
      } else {
        GfxSpriteSetQuadMode(sprite, 2);
        sprite->posX = (float)entry->x;
        sprite->posW = 0.0f;
        sprite->posY = (float)entry->y;
        sprite->posZ = (float)entry->z;
        UiSpriteSetUvRectAndSize((float)entry->u, (float)entry->v, (float)entry->w, (float)entry->h,
                                 self, sprite);
      }
    }
    for (i = 21; i < 23; i++) {
      sprite = g_uiLoadingShared->sprites[i];
      sprite->posX = sprite->posX + 32.0f;
      if (i == 21) {
        GfxSpriteCenterPivot(sprite);
        sprite->maybe_billboardParams80[1] = 0.6f;
        sprite->maybe_billboardParams80[0] = 0.6f;
        GfxSpriteSetScaleRotation(sprite, 0.6f, sprite->maybe_billboardParams80[1],
                                  sprite->maybe_billboardParams80[2], false);
      }
    }
    if (UiLoadingIsPropellerTheme(self)) {
      UiLoadingPickBackground(self);
      g_uiLoadingShared->sprites[21]->flags &= ~1u;
      g_uiLoadingShared->sprites[22]->flags &= ~1u;
    } else {
      g_uiLoadingShared->sprites[21]->alpha = 0.0f;
      g_uiLoadingShared->sprites[21]->flags |= 1;
      g_uiLoadingShared->sprites[22]->flags |= 1;
    }
    if (UiLoadingHasBallTexture(self)) {
      self->iconGame = 1;
      for (i = 11; i < 17; i++) {
        g_uiLoadingShared->sprites[i]->flags |= 1;
      }
    } else {
      for (i = 11; i < 17; i++) {
        g_uiLoadingShared->sprites[i]->flags &= ~1u;
      }
    }
    if (SaveHasProfile()) {
      sprite = g_uiLoadingShared->sprites[9];
      lang = SaveProfileGetLanguage(SaveGetProfile());
      GfxSpriteSetVCell((float)g_loadingLangCells[lang], sprite);
      sprite = g_uiLoadingShared->sprites[22];
      lang = SaveProfileGetLanguage(SaveGetProfile());
      GfxSpriteSetVCell((float)g_loadingLangCells[lang], sprite);
    }
    if (g_uiLoadingShared->fab == NULL) {
      fab = NULL;
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      mem = MemAlloc(0xb0, NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      shared = g_uiLoadingShared;
      if (mem != NULL) {
        GfxFabCtor((GfxFab *)mem, "nowloadingflash.fab", g_nowLoadingFlashFabData, &shared->fabList);
        fab = (GfxFab *)mem;
        shared = g_uiLoadingShared;
      }
      shared->fab = fab;
      g_uiLoadingShared->fab->depth = 1510.0f;
      g_uiLoadingShared->fab->loop = 1;
      g_uiLoadingShared->fabFrame = 0;
      g_uiLoadingShared->fabFrameCount = GfxFabGetFrameCount(g_uiLoadingShared->fab);
    }
    self->step = self->step + 1;
  } else if (g_uiLoadingShared->fab == NULL) {
    self->step = 1;
  } else {
    if (UiTextBoxHasPrinter(g_uiLoadingShared->box)) {
      text = self->tipText;
      UiTextMeasure(0.0f, UiTextBoxGetPrinter(g_uiLoadingShared->box), text, &self->tipWidth,
                    &self->tipHeight, NULL);
      width = self->tipWidth;
      printer = (UiTextPrinter *)UiTextBoxGetPrinter(g_uiLoadingShared->box);
      if (width <= printer->lineHeight * 3.0f) {
        UiTextBoxPrint(g_uiLoadingShared->box, 0xf0, 0xc0, text, 1, 0);
      } else {
        width = self->tipWidth;
        printer = (UiTextPrinter *)UiTextBoxGetPrinter(g_uiLoadingShared->box);
        if (width <= printer->lineHeight * 4.0f) {
          UiTextBoxPrint(g_uiLoadingShared->box, 0xf0, 0xba, text, 1, 0);
        } else {
          UiTextBoxPrint(g_uiLoadingShared->box, 0xf0, 0xb4, text, 1, 0);
        }
      }
    }
    GfxFabSeek(g_uiLoadingShared->fab, 0);
    self->tipScroll = 0.0f;
    self->tipScrollEnd = -1.0f;
    self->state = self->state + 1;
    self->step = 0;
  }
  if (UiTextBoxHasPrinter(g_uiLoadingShared->box)) {
    UiLoadingSyncTipScroll(self);
  }
}
