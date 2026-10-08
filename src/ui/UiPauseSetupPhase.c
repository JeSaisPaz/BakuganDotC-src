// bdc 0x0891122c UiPauseSetupPhase
#include "bdc.h"

/* Phase 1 of the pause menu (runs once, at phaseStep 0): counts the entries of the current kind
   (`g_uiPauseItemActions`, up to 7, stopping at -1), creates the menu sprites of layout 0x12
   (`UiLayoutCreateSprites`) plus a highlight sprite allocated from low memory into the sprite
   table slot 36, hides the optional sprites, shows the gimmick counters for kind 4 (field), prepares
   the entry frames/labels (centred, inset, label cells from `g_uiPauseActionCells`, action 0xd greyed
   out), lays them out (`UiPauseLayoutEntries`), puts the cursor on action 0xe when the previous
   screen was task 0x12d, and fills hintText[1] depending on the kind: the talk window's message
   (kinds 0, 1, 3, 10), line 10 of `"DMPauseEnd"` (kind 2) or the current story hint from
   `"mes_Adventure_hint_<lang>.bin"` (kind 4). Every call then runs `UiPauseInitPlayerBadge`, resets
   phaseStep and advances phase. */

#define SPR(n) (((GfxSprite **)self->base.data)[n])

void UiPauseSetupPhase(UiPause *self)
{
  GfxSprite *sprite;
  GameGimmick *gimmick;
  CoreTask *field;
  void *mes;
  u32 mesCount;
  int i;
  int total;
  int inactive;
  int action;
  float w;
  float h;
  char path[64];
  s32 chapter;
  s16 hint;

  if (self->base.phaseStep == 0) {
    self->entryCount = 0;
    for (i = 0; i < 7; ) {
      if (g_uiPauseItemActions[self->kind][i] == -1) {
        break;
      }
      i++;
      self->entryCount = i;
    }
    UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x12);

    {
      bool fromLow;

      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      sprite = (GfxSprite *)MemAlloc(sizeof(GfxSprite), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
    }
    if (sprite != NULL) {
      GfxSpriteCtor(sprite);
    }
    SPR(36) = sprite;
    SPR(36)->flags &= ~1u;
    GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)SPR(36));
    CoreTaskExists(100);
    SPR(2)->flags &= ~1u;
    SPR(3)->flags &= ~1u;
    SPR(4)->flags &= ~1u;
    SPR(20)->flags &= ~1u;
    SPR(21)->flags &= ~1u;
    SPR(22)->flags &= ~1u;
    SPR(23)->flags &= ~1u;
    SPR(34)->flags &= ~1u;
    SPR(35)->flags &= ~1u;

    if (self->kind == 4) {
      field = GameFieldFindTask();
      gimmick = ((GameFieldTask *)field)->gimmicks;
      total = 0;
      inactive = 0;
      sprite = SPR(29);
      while (gimmick != NULL) {
        if (gimmick->typeId == 0xbdf) {
          if (gimmick->active == 0) {
            inactive++;
          }
          total++;
        }
        gimmick = (GameGimmick *)gimmick->base.base.next;
      }
      GfxSpriteSetCell(sprite, (float)((inactive / 10) / 6), (float)((inactive / 10) % 6));
      GfxSpriteSetCell(SPR(30), (float)((inactive % 10) / 6), (float)((inactive % 10) % 6));
      GfxSpriteSetCell(SPR(32), (float)((total / 10) / 6), (float)((total / 10) % 6));
      GfxSpriteSetCell(SPR(33), (float)((total % 10) / 6), (float)((total % 10) % 6));
      SPR(28)->posZ = -100.0f;
      SPR(29)->posZ = -100.0f;
      SPR(30)->posZ = -100.0f;
      SPR(31)->posZ = -100.0f;
      SPR(32)->posZ = -100.0f;
      SPR(33)->posZ = -100.0f;
    } else {
      SPR(28)->flags &= ~1u;
      SPR(29)->flags &= ~1u;
      SPR(30)->flags &= ~1u;
      SPR(31)->flags &= ~1u;
      SPR(32)->flags &= ~1u;
      SPR(33)->flags &= ~1u;
    }
    GfxSpriteSetCell(SPR(24), 0.0f, 2.0f);
    GfxSpriteSetCell(SPR(25), 0.0f, 1.0f);

    for (i = 0; i < self->spriteCount; i++) {
      SPR(i)->alpha = 0.0f;
    }
    for (i = 0; i < 7; i++) {
      sprite = SPR(5 + i);
      if (i < self->entryCount) {
        GfxSpriteCenterPivot(sprite);
        GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f, false);
        GfxSpriteInsetUv(0.5f, sprite);
      } else {
        sprite->flags &= ~1u;
      }
      sprite = SPR(13 + i);
      if (i < self->entryCount) {
        GfxSpriteCenterPivot(sprite);
        GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, 0.0f, false);
        GfxSpriteInsetUv(0.5f, sprite);
      } else {
        sprite->flags &= ~1u;
      }
    }
    GfxSpriteCenterPivot(SPR(20));
    GfxSpriteSetScaleRotation(SPR(20), 1.0f, 1.0f, 0.0f, false);
    GfxSpriteInsetUv(0.5f, SPR(20));
    w = GfxSpriteGetWidth(SPR(20)) * 1.1f;
    sprite = SPR(20);
    h = GfxSpriteGetHeight(sprite);
    UiSpriteSetSize(w, h * 1.1f, sprite);
    UiPauseLayoutEntries(self, self->entryCount);

    for (i = 0; i < self->entryCount; i++) {
      action = g_uiPauseItemActions[self->kind][i];
      GfxSpriteSetCell(SPR(13 + i), (float)g_uiPauseActionCells[action][0],
                       (float)g_uiPauseActionCells[action][1]);
      if (g_uiPauseItemActions[self->kind][i] == 0xd) {
        SPR(13 + i)->tint[0] = 0.3f;
        SPR(13 + i)->tint[1] = 0.3f;
        SPR(13 + i)->tint[2] = 0.3f;
        SPR(13 + i)->alpha = 1.0f;
      }
    }

    if (self->kind >= 6 && self->kind < 10) {
      GfxSpriteSetCell(SPR(12), 0.0f, 3.0f);
      SPR(25)->flags &= ~1u;
      SPR(27)->flags &= ~1u;
      if (SaveGetProfileFlag0() != 0 && NetGetLocalPlayerIndex() == 1) {
        SPR(24)->flags &= ~1u;
        SPR(26)->flags &= ~1u;
      }
    }

    if (g_lastScreenTaskId == 0x12d) {
      for (i = 0; i < self->entryCount; i++) {
        if (g_uiPauseItemActions[self->kind][i] == 0xe) {
          self->cursor = i;
          break;
        }
      }
    }

    switch (self->kind) {
    case 0:
    case 1:
    case 3:
    case 10:
      strcpy(self->hintText[1], "");
      if (UiTalkTaskExists() != 0) {
        strcpy(self->hintText[1], UiGetTalkTask()->msgText);
      }
      if (strcmp(self->hintText[1], "") != 0) {
        SPR(21)->flags |= 1;
      }
      break;
    case 2:
      mes = SaveFindLocalizedBin("DMPauseEnd");
      if ((s32)UiMesTableRelocate(mes) > 10) {
        strcpy(self->hintText[1], (const char *)PspPtr(((u32 *)mes)[10]));
      }
      if (strcmp(self->hintText[1], "") != 0) {
        SPR(34)->flags |= 1;
      }
      break;
    case 4:
      SPR(23)->flags |= 1;
      field = (CoreTask *)CoreTaskFind(500);
      if (field != NULL) {
        sprintf(path, "mes_Adventure_hint_%s.bin", SaveGetLanguageName());
        mes = CorePackChainFind(g_ioLzsPackages, path);
        mesCount = UiMesTableRelocate(mes);
        chapter = -1;
        hint = 0;
        GameFieldGetStoryHint(field, &chapter, &hint);
        if ((s32)(u16)hint < (s32)mesCount) {
          strcpy(self->hintText[1], (const char *)PspPtr(((u32 *)mes)[(u16)hint]));
        }
      }
      break;
    default:
      break;
    }
    self->base.phaseStep++;
  }
  UiPauseInitPlayerBadge(self);
  self->base.phaseStep = 0;
  self->base.phase++;
}

#undef SPR
