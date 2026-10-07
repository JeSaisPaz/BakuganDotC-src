// bdc 0x089947f4 UiUnlockCodeInputPhase
#include "bdc.h"

/* Phase 3 of `UiUnlockCode`: the code-entry keyboard. Button 0x2000 leaves
   (phase 5, sound 2) on a press with an empty code or on a repeat with a non-empty one. Button
   0x4000 confirms (repeat on character keys, space and backspace; press on the other function
   keys and OK): a character key (rows 0-3, `pageOffset` or 0xa0 on the extra page + column +
   row*10) or space (table entry 0x27) is stored at the caret (`entered[]`, caret advances while
   below `digitCount`-1, `UiUnlockCodeLayoutDigits`, key pop 10), except a blank at caret 0
   (sound 3); function row 4 runs `UiUnlockCodeSwitchKeyPageA` / `UiUnlockCodeSwitchKeyPageB`,
   toggles the extra page `page`, enters space, or backspaces (sound 8 when empty); OK (row 5)
   with a non-empty code sets `inputFlag`, goes to the check phase 4 and returns at once.
   D-pad repeat (0x20/0x80 column ±1 mod 10 or mod 5, 0x40/0x10 row ±1 mod 6, mapping columns
   through the function-key tables on row 4) plays sound 1. Every other frame path then dims the
   OK/backspace (empty code) and space (caret 0) keys, places the cursor sprites (data sprites
   10/11 for characters, 16/17/20/21 for function keys, 27 for OK) and the caret marker 26,
   runs `UiUnlockCodeUpdateKeyPop`, pulses the cursors with cos(`cursorAngle`), advances
   `cursorAngle` by 20 mod 360 and, on the OK row, runs the OK flash (`okFlashT` += 0.05,
   sprite 32 reset from 27 once it reaches 1.0).
   The cursor cosines are vcos.s of angle * S703 (2/pi), i.e. cosf of the radian angle. */

static inline GfxSprite *UiUnlockCodeInputSprite(UiUnlockCode *self, int index)
{
  return ((GfxSprite **)self->base.data)[index];
}

/* lv.q/sv.q of a key colour: tint[0..2] and alpha in one quad */
static inline void UiUnlockCodeSetKeyColor(GfxSprite *sprite, const ScePspFVector4 *color)
{
  sprite->tint[0] = color->x;
  sprite->tint[1] = color->y;
  sprite->tint[2] = color->z;
  sprite->alpha = color->w;
}

void UiUnlockCodeInputPhase(UiUnlockCode *self)

{
  s32 colToFunc[10];
  s32 funcToCol[5];
  SndManager *mgr;
  GfxSprite *sprite;
  int caret;
  int count;
  int key;
  int row;
  int i;
  float cosA;
  float glowCursor;
  float glowKey;
  float glowFrame;
  float scale;
  float width;
  float height;
  float angle;
  float t;

  /* character column -> function key under it, and back */
  colToFunc[0] = 0;
  colToFunc[1] = 1;
  colToFunc[2] = 1;
  colToFunc[3] = 2;
  colToFunc[4] = 2;
  colToFunc[5] = 3;
  colToFunc[6] = 3;
  colToFunc[7] = 3;
  colToFunc[8] = 4;
  colToFunc[9] = 4;
  funcToCol[0] = 0;
  funcToCol[1] = 1;
  funcToCol[2] = 3;
  funcToCol[3] = 5;
  funcToCol[4] = 8;

  if (((self->base.pad->pressed & 0x2000) && self->entered[0] == -1) ||
      ((self->base.pad->repeat & 0x2000) && self->entered[0] != -1)) {
    self->inputFlag = 0;
    self->base.phaseStep = 0;
    self->base.phase = 5;
    if (SndHasManager()) {
      mgr = SndGetManager();
      SndManagerPlay(mgr, 2, 0, 0);
    }
  }
  else if (((self->base.pad->repeat & 0x4000) &&
            (self->keyRow < 4 || (self->keyRow == 4 && self->keyColumn >= 3))) ||
           ((self->base.pad->pressed & 0x4000) && self->keyRow >= 4)) {
    if (self->keyRow == 4) {
      switch (self->keyColumn) {
      case 0:
        UiUnlockCodeSwitchKeyPageA(self);
        UiUnlockCodeStartKeyPop(self, 0x11);
        break;
      case 1:
        UiUnlockCodeSwitchKeyPageB(self);
        UiUnlockCodeStartKeyPop(self, 0x11);
        break;
      case 2:
        self->page = self->page == 0;
        UiUnlockCodeLayoutKeyboard(self);
        if (SndHasManager()) {
          mgr = SndGetManager();
          SndManagerPlay(mgr, 6, 0, 0);
        }
        UiUnlockCodeStartKeyPop(self, 0x10);
        break;
      case 3:
        if (strcmp(g_unlockKeyTable[0x27], g_unlockSpaceString) == 0 && self->caret == 0) {
          if (SndHasManager()) {
            mgr = SndGetManager();
            SndManagerPlay(mgr, 3, 0, 0);
          }
        }
        else {
          if (SndHasManager()) {
            mgr = SndGetManager();
            SndManagerPlay(mgr, 0, 0, 0);
          }
          caret = self->caret;
          count = self->digitCount;
          self->entered[caret] = 0x27;
          if (caret + 1 < count) {
            self->caret = self->caret + 1;
          }
          UiUnlockCodeLayoutDigits(self);
          UiUnlockCodeStartKeyPop(self, 10);
        }
        UiUnlockCodeStartKeyPop(self, 0x10);
        break;
      case 4:
        if (self->entered[0] == -1) {
          if (SndHasManager()) {
            mgr = SndGetManager();
            SndManagerPlay(mgr, 8, 0, 0);
          }
        }
        else {
          if (SndHasManager()) {
            mgr = SndGetManager();
            SndManagerPlay(mgr, 2, 0, 0);
          }
          /* a filled last slot is erased in place, otherwise step back first */
          if (self->caret > 0 &&
              (self->caret != self->digitCount - 1 || self->entered[self->caret] == -1)) {
            self->caret = self->caret - 1;
          }
          i = self->caret;
          self->entered[i] = -1;
          for (; i < self->digitCount - 1; i++) {
            self->entered[i] = self->entered[i + 1];
          }
          self->entered[self->digitCount - 1] = -1;
          UiUnlockCodeLayoutDigits(self);
        }
        UiUnlockCodeStartKeyPop(self, 0x10);
        break;
      default:
        break;
      }
    }
    else if (self->keyRow == 5) {
      if (self->entered[0] != -1) {
        self->inputFlag = 1;
        self->base.phaseStep = 0;
        if (SndHasManager()) {
          mgr = SndGetManager();
          SndManagerPlay(mgr, 0, 0, 0);
        }
        self->base.phase = 4;
        UiUnlockCodeStartKeyPop(self, 0x1b);
        sprite = UiUnlockCodeInputSprite(self, 32);
        sprite->flags = sprite->flags & ~1u;
        sprite = UiUnlockCodeInputSprite(self, 13);
        sprite->addColor[0] = 0.2f;
        sprite->addColor[1] = 0.2f;
        sprite->addColor[2] = 0.2f;
        sprite->addColor[3] = 1.0f;
        return;
      }
      if (SndHasManager()) {
        mgr = SndGetManager();
        SndManagerPlay(mgr, 8, 0, 0);
      }
    }
    else {
      key = self->pageOffset;
      if (self->page != 0) {
        key = 0xa0;
      }
      key = key + self->keyColumn + self->keyRow * 10;
      if (strcmp(g_unlockKeyTable[key], g_unlockSpaceString) == 0 && self->caret == 0) {
        if (SndHasManager()) {
          mgr = SndGetManager();
          SndManagerPlay(mgr, 3, 0, 0);
        }
      }
      else {
        if (SndHasManager()) {
          mgr = SndGetManager();
          SndManagerPlay(mgr, 0, 0, 0);
        }
        caret = self->caret;
        count = self->digitCount;
        self->entered[caret] = key;
        if (caret + 1 < count) {
          self->caret = self->caret + 1;
        }
        UiUnlockCodeLayoutDigits(self);
        UiUnlockCodeStartKeyPop(self, 10);
      }
    }
  }
  else if (self->base.pad->repeat & 0x20) {
    if (self->keyRow < 4) {
      self->keyColumn = (self->keyColumn + 1) % 10;
    }
    else if (self->keyRow == 4) {
      self->keyColumn = (self->keyColumn + 1) % 5;
    }
    if (SndHasManager()) {
      mgr = SndGetManager();
      SndManagerPlay(mgr, 1, 0, 0);
    }
  }
  else if (self->base.pad->repeat & 0x80) {
    if (self->keyRow < 4) {
      self->keyColumn = (self->keyColumn + 9) % 10;
    }
    else if (self->keyRow == 4) {
      self->keyColumn = (self->keyColumn + 4) % 5;
    }
    if (SndHasManager()) {
      mgr = SndGetManager();
      SndManagerPlay(mgr, 1, 0, 0);
    }
  }
  else if (self->base.pad->repeat & 0x40) {
    if (self->keyRow == 4) {
      self->keyColumn = funcToCol[self->keyColumn];
    }
    row = (self->keyRow + 1) % 6;
    self->keyRow = row;
    if (row == 4) {
      self->keyColumn = colToFunc[self->keyColumn];
    }
    if (SndHasManager()) {
      mgr = SndGetManager();
      SndManagerPlay(mgr, 1, 0, 0);
    }
  }
  else if (self->base.pad->repeat & 0x10) {
    if (self->keyRow == 4) {
      self->keyColumn = funcToCol[self->keyColumn];
    }
    row = (self->keyRow + 5) % 6;
    self->keyRow = row;
    if (row == 4) {
      self->keyColumn = colToFunc[self->keyColumn];
    }
    if (SndHasManager()) {
      mgr = SndGetManager();
      SndManagerPlay(mgr, 1, 0, 0);
    }
  }

  /* OK and backspace dim while the code is empty */
  if (self->entered[0] == -1) {
    UiUnlockCodeInputSprite(self, 12)->alpha = 0.5f;
    sprite = UiUnlockCodeInputSprite(self, 13);
    sprite->tint[0] = 0.5f;
    sprite->tint[1] = 0.5f;
    sprite->tint[2] = 0.5f;
    sprite->alpha = 1.0f;
    UiUnlockCodeSetKeyColor(UiUnlockCodeInputSprite(self, 25), &g_unlockCodeColorDimGreen);
    UiUnlockCodeInputSprite(self, 9)->alpha = 0.5f;
  }
  else {
    UiUnlockCodeInputSprite(self, 12)->alpha = 1.0f;
    sprite = UiUnlockCodeInputSprite(self, 13);
    sprite->tint[0] = 1.0f;
    sprite->tint[1] = 1.0f;
    sprite->tint[2] = 1.0f;
    sprite->alpha = 1.0f;
    UiUnlockCodeSetKeyColor(UiUnlockCodeInputSprite(self, 25), &g_unlockCodeColorDarkGreen);
    UiUnlockCodeInputSprite(self, 9)->alpha = 1.0f;
  }
  /* space dims at caret 0 */
  if (self->caret == 0) {
    UiUnlockCodeSetKeyColor(UiUnlockCodeInputSprite(self, 24), &g_unlockCodeColorDimGreen);
    UiUnlockCodeInputSprite(self, 8)->alpha = 0.5f;
  }
  else {
    UiUnlockCodeSetKeyColor(UiUnlockCodeInputSprite(self, 24), &g_unlockCodeColorDarkGreen);
    UiUnlockCodeInputSprite(self, 8)->alpha = 1.0f;
  }
  /* sprite 13's textureSlot word doubles as "OK not highlighted" (cleared on the OK row) */
  UiUnlockCodeInputSprite(self, 13)->textureSlot = 1;

  if (self->keyRow < 4) {
    UiUnlockCodeInputSprite(self, 11)->posX = (float)self->keyColumn * 28.0f + 91.0f;
    UiUnlockCodeInputSprite(self, 11)->posY = (float)self->keyRow * 26.0f + 63.5f;
    UiUnlockCodeInputSprite(self, 10)->posX = UiUnlockCodeInputSprite(self, 11)->posX + 21.0f;
    UiUnlockCodeInputSprite(self, 10)->posY = UiUnlockCodeInputSprite(self, 11)->posY + 20.0f;
    sprite = UiUnlockCodeInputSprite(self, 10);
    sprite->flags = sprite->flags | 1;
    sprite = UiUnlockCodeInputSprite(self, 16);
    sprite->flags = sprite->flags & ~1u;
    sprite = UiUnlockCodeInputSprite(self, 17);
    sprite->flags = sprite->flags & ~1u;
    sprite = UiUnlockCodeInputSprite(self, 11);
    sprite->flags = sprite->flags | 1;
    sprite = UiUnlockCodeInputSprite(self, 20);
    sprite->flags = sprite->flags & ~1u;
    sprite = UiUnlockCodeInputSprite(self, 21);
    sprite->flags = sprite->flags & ~1u;
    sprite = UiUnlockCodeInputSprite(self, 27);
    sprite->flags = sprite->flags & ~1u;
    key = self->pageOffset;
    if (self->page != 0) {
      key = 0xa0;
    }
    if (strcmp(g_unlockKeyTable[key + self->keyColumn + self->keyRow * 10], g_unlockSpaceString) == 0 &&
        self->caret == 0) {
      sprite = UiUnlockCodeInputSprite(self, 11);
      sprite->flags = sprite->flags & ~1u;
    }
  }
  else if (self->keyRow == 4) {
    if (self->keyColumn < 2) {
      /* narrow cursor 17 / highlight 21 over page keys 14, 15 */
      UiUnlockCodeInputSprite(self, 21)->posX =
          UiUnlockCodeInputSprite(self, 14 + self->keyColumn)->posX - 1.0f;
      t = UiUnlockCodeInputSprite(self, 14 + self->keyColumn)->posX - 2.0f;
      width = GfxSpriteGetWidth(UiUnlockCodeInputSprite(self, 17));
      UiUnlockCodeInputSprite(self, 17)->posX = t + width * 0.5f;
      sprite = UiUnlockCodeInputSprite(self, 16);
      sprite->flags = sprite->flags & ~1u;
      sprite = UiUnlockCodeInputSprite(self, 17);
      sprite->flags = sprite->flags | 1;
      sprite = UiUnlockCodeInputSprite(self, 20);
      sprite->flags = sprite->flags & ~1u;
      sprite = UiUnlockCodeInputSprite(self, 21);
      sprite->flags = sprite->flags | 1;
    }
    else {
      /* wide cursor 16 / highlight 20 over keys 23 (page), 24 (space), 25 (backspace) */
      UiUnlockCodeInputSprite(self, 20)->posX =
          UiUnlockCodeInputSprite(self, 21 + self->keyColumn)->posX;
      t = UiUnlockCodeInputSprite(self, 21 + self->keyColumn)->posX;
      width = GfxSpriteGetWidth(UiUnlockCodeInputSprite(self, 16));
      UiUnlockCodeInputSprite(self, 16)->posX = t + width * 0.5f + 0.5f;
      sprite = UiUnlockCodeInputSprite(self, 16);
      sprite->flags = sprite->flags | 1;
      sprite = UiUnlockCodeInputSprite(self, 17);
      sprite->flags = sprite->flags & ~1u;
      sprite = UiUnlockCodeInputSprite(self, 20);
      sprite->flags = sprite->flags | 1;
      sprite = UiUnlockCodeInputSprite(self, 21);
      sprite->flags = sprite->flags & ~1u;
      if ((self->entered[0] == -1 && self->keyColumn == 3) ||
          (self->caret == 0 && self->keyColumn == 4)) {
        sprite = UiUnlockCodeInputSprite(self, 20);
        sprite->flags = sprite->flags & ~1u;
      }
    }
    sprite = UiUnlockCodeInputSprite(self, 10);
    sprite->flags = sprite->flags & ~1u;
    sprite = UiUnlockCodeInputSprite(self, 11);
    sprite->flags = sprite->flags & ~1u;
    sprite = UiUnlockCodeInputSprite(self, 27);
    sprite->flags = sprite->flags & ~1u;
  }
  else if (self->keyRow == 5) {
    sprite = UiUnlockCodeInputSprite(self, 10);
    sprite->flags = sprite->flags & ~1u;
    sprite = UiUnlockCodeInputSprite(self, 16);
    sprite->flags = sprite->flags & ~1u;
    sprite = UiUnlockCodeInputSprite(self, 17);
    sprite->flags = sprite->flags & ~1u;
    sprite = UiUnlockCodeInputSprite(self, 11);
    sprite->flags = sprite->flags & ~1u;
    sprite = UiUnlockCodeInputSprite(self, 20);
    sprite->flags = sprite->flags & ~1u;
    sprite = UiUnlockCodeInputSprite(self, 21);
    sprite->flags = sprite->flags & ~1u;
    sprite = UiUnlockCodeInputSprite(self, 27);
    sprite->flags = sprite->flags | 1;
    if (self->entered[0] != -1) {
      UiUnlockCodeInputSprite(self, 13)->textureSlot = 0;
    }
  }

  UiUnlockCodeInputSprite(self, 26)->posX = (float)self->caret * 21.0f + 104.5f;
  UiUnlockCodeUpdateKeyPop(self);

  /* cursor pulses: cos of the radian angle */
  angle = (float)self->cursorAngle * 0.0055555557f * 3.1415927f;
  cosA = __builtin_cosf(angle);
  glowCursor = (1.0f - cosA) * 0.5f * 0.5f;
  t = (float)(self->cursorAngle + 60) * 0.0055555557f;
  angle = t * 3.1415927f;
  cosA = __builtin_cosf(angle);
  glowKey = (1.0f - cosA) * 0.5f * 0.8f;
  angle = t * 3.1415927f;
  cosA = __builtin_cosf(angle);
  glowFrame = (1.0f - cosA) * 0.5f * 0.1f;

  sprite = UiUnlockCodeInputSprite(self, 26);
  sprite->addColor[0] = glowCursor;
  sprite->addColor[1] = glowCursor;
  sprite->addColor[2] = glowCursor;
  sprite->addColor[3] = 1.0f;
  sprite = UiUnlockCodeInputSprite(self, 11);
  sprite->addColor[0] = glowKey;
  sprite->addColor[1] = glowKey;
  sprite->addColor[2] = glowKey;
  sprite->addColor[3] = 1.0f;
  sprite = UiUnlockCodeInputSprite(self, 20);
  sprite->addColor[0] = glowKey;
  sprite->addColor[1] = glowKey;
  sprite->addColor[2] = glowKey;
  sprite->addColor[3] = 1.0f;
  sprite = UiUnlockCodeInputSprite(self, 21);
  sprite->addColor[0] = glowKey;
  sprite->addColor[1] = glowKey;
  sprite->addColor[2] = glowKey;
  sprite->addColor[3] = 1.0f;
  sprite = UiUnlockCodeInputSprite(self, 10);
  sprite->addColor[0] = glowFrame;
  sprite->addColor[1] = glowFrame;
  sprite->addColor[2] = glowFrame;
  sprite->addColor[3] = 1.0f;
  sprite = UiUnlockCodeInputSprite(self, 16);
  sprite->addColor[0] = glowFrame;
  sprite->addColor[1] = glowFrame;
  sprite->addColor[2] = glowFrame;
  sprite->addColor[3] = 1.0f;
  sprite = UiUnlockCodeInputSprite(self, 17);
  sprite->addColor[0] = glowFrame;
  sprite->addColor[1] = glowFrame;
  sprite->addColor[2] = glowFrame;
  sprite->addColor[3] = 1.0f;

  /* cursor frames breathe by up to 5% */
  angle = (float)(self->cursorAngle + 60) * 0.0055555557f * 3.1415927f;
  cosA = __builtin_cosf(angle);
  scale = (1.0f - cosA) * 0.5f * 0.05f + 1.0f;
  width = GfxSpriteGetWidth(UiUnlockCodeInputSprite(self, 10)) * scale;
  sprite = UiUnlockCodeInputSprite(self, 10);
  height = GfxSpriteGetHeight(sprite);
  GfxSpriteSetSize(sprite, width, height * scale);
  width = GfxSpriteGetWidth(UiUnlockCodeInputSprite(self, 16)) * scale;
  sprite = UiUnlockCodeInputSprite(self, 16);
  height = GfxSpriteGetHeight(sprite);
  GfxSpriteSetSize(sprite, width, height * scale);
  width = GfxSpriteGetWidth(UiUnlockCodeInputSprite(self, 17)) * scale;
  sprite = UiUnlockCodeInputSprite(self, 17);
  height = GfxSpriteGetHeight(sprite);
  GfxSpriteSetSize(sprite, width, height * scale);
  sprite = UiUnlockCodeInputSprite(self, 10);
  sprite->flags = sprite->flags | 0x20;
  sprite = UiUnlockCodeInputSprite(self, 16);
  sprite->flags = sprite->flags | 0x20;
  sprite = UiUnlockCodeInputSprite(self, 17);
  sprite->flags = sprite->flags | 0x20;
  self->cursorAngle = (self->cursorAngle + 20) % 360;

  sprite = UiUnlockCodeInputSprite(self, 13);
  sprite->addColor[0] = 0.0f;
  sprite->addColor[1] = 0.0f;
  sprite->addColor[2] = 0.0f;
  sprite->addColor[3] = 1.0f;
  sprite = UiUnlockCodeInputSprite(self, 32);
  sprite->flags = sprite->flags & ~1u;
  if (self->keyRow == 5) {
    if (UiUnlockCodeInputSprite(self, 13)->textureSlot == 0) {
      glowKey = glowKey * 0.5f;
      sprite = UiUnlockCodeInputSprite(self, 13);
      sprite->addColor[0] = glowKey;
      sprite->addColor[1] = glowKey;
      sprite->addColor[2] = glowKey;
      sprite->addColor[3] = 1.0f;
    }
    sprite = UiUnlockCodeInputSprite(self, 32);
    sprite->flags = sprite->flags | 1;
    /* OK flash: sprite 32 grows from sprite 27's size and fades out over 20 frames */
    sprite = UiUnlockCodeInputSprite(self, 32);
    width = GfxSpriteGetWidth(UiUnlockCodeInputSprite(self, 27)) * (self->okFlashT * 0.15f + 1.0f);
    height = GfxSpriteGetHeight(UiUnlockCodeInputSprite(self, 27));
    UiSpriteSetSize(width, height * (self->okFlashT * 0.1f + 1.0f), sprite);
    t = self->okFlashT;
    sprite = UiUnlockCodeInputSprite(self, 32);
    sprite->addColor[0] = t;
    sprite->addColor[1] = t;
    sprite->addColor[2] = t;
    sprite->addColor[3] = 1.0f;
    UiUnlockCodeInputSprite(self, 32)->alpha = 1.0f - self->okFlashT;
    t = self->okFlashT + 0.05f;
    self->okFlashT = t;
    if (!(t < 1.0f)) {
      GfxSpriteCopy(UiUnlockCodeInputSprite(self, 27), UiUnlockCodeInputSprite(self, 32));
      self->okFlashT = 0.0f;
    }
  }
  else {
    sprite = UiUnlockCodeInputSprite(self, 32);
    sprite->flags = sprite->flags & ~1u;
    self->okFlashT = 0.0f;
  }
}
