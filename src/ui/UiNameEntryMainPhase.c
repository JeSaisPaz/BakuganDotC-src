// bdc 0x088068f4 UiNameEntryMainPhase
#include "bdc.h"

/* Phase 3 of the name entry screen (`UiNameEntry`): the keyboard input loop.
   Pad bits (PadState `pressed`/`repeat`): 0x2000 pressed with an empty name, or repeating with a
   non-empty one, leaves with `confirmed = 0` (phase 4, sound 2). 0x4000 activates the key under
   the cursor (auto-repeats on the 10x4 grid and on bottom-row keys 3/4, needs a fresh press on the
   page keys and on OK): grid keys type key `page (0xa0 on the symbol page) + col + row*10` (sound
   0; a blank first character buzzes, sound 3); bottom row 0/1 swap the page (half/quarter), 2
   toggles the symbol page (sound 6), 3 types a space (key 0x27), 4 deletes the last character
   (sound 2, or 8 on an empty name); row 5 (OK) on a non-empty name sets `confirmed = 1`, goes to
   phase 4 (sound 0), starts the key-press pop on sprite 26 and returns at once, on an empty name
   it buzzes (sound 8). D-pad repeat bits 0x20/0x80 move the column (mod 10 on the grid, mod 5 on
   the bottom row), 0x40/0x10 move the row (mod 6), mapping the column between grid and bottom row
   (sound 1). Then every frame: dims layout sprites 9/12/13/24 while the name is empty and 8/23
   while nothing is typed (tints g_nameEntryColorDimGreen vs g_nameEntryColorDarkGreen), places
   the cursor sprites for the current row, runs `UiNameEntryAnimateKeyPress`, pulses the cursor colours and scale with
   cos(cursorAngle) (`cursorAngle += 20` mod 360) and, on row 5, grows/fades the OK flash sprite 34
   by `flashProgress += 0.05` until it reaches 1.0.
   The cosines are VFPU `vcos.s` of `angle * S703` (bank 2/π): quarter turns that cancel, so plain
   cos(angle in radians). */

static inline void NameEntryCopyTint(GfxSprite *sprite, const ScePspFVector4 *color)
{
  /* 16-byte copy (lv.q/sv.q) into tint[0..2] + alpha */
  sprite->tint[0] = color->x;
  sprite->tint[1] = color->y;
  sprite->tint[2] = color->z;
  sprite->alpha = color->w;
}

void UiNameEntryMainPhase(UiNameEntry *self)
{
  s32 gridToBottom[10];
  s32 bottomToGrid[5];
  GfxSprite **sprites;
  GfxSprite *sprite;
  s32 key;
  s32 len;
  s32 i;
  s32 angle;
  float x;
  float w;
  float h;
  float t;
  float pulseName;
  float pulseCursor;
  float pulseFrame;
  float scale;
  float flash;

  gridToBottom[0] = 0;
  gridToBottom[1] = 1;
  gridToBottom[2] = 1;
  gridToBottom[3] = 2;
  gridToBottom[4] = 2;
  gridToBottom[5] = 3;
  gridToBottom[6] = 3;
  gridToBottom[7] = 3;
  gridToBottom[8] = 4;
  gridToBottom[9] = 4;
  bottomToGrid[0] = 0;
  bottomToGrid[1] = 1;
  bottomToGrid[2] = 3;
  bottomToGrid[3] = 5;
  bottomToGrid[4] = 8;

  if (((self->base.pad->pressed & 0x2000) != 0 && self->name[0] == -1) ||
      ((self->base.pad->repeat & 0x2000) != 0 && self->name[0] != -1)) {
    /* back out */
    self->confirmed = 0;
    self->base.phaseStep = 0;
    self->base.phase = 4;
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 2, 0, 0);
    }
  }
  else if (((self->base.pad->repeat & 0x4000) != 0 &&
            (self->keyRow < 4 || (self->keyRow == 4 && self->keyCol >= 3))) ||
           ((self->base.pad->pressed & 0x4000) != 0 && self->keyRow >= 4)) {
    /* activate the key under the cursor */
    if (self->keyRow == 4) {
      switch (self->keyCol) {
      case 0:
        UiNameEntrySwapPageHalf(self);
        UiNameEntryStartKeyPress(self, 0x11);
        break;
      case 1:
        UiNameEntrySwapPageQuarter(self);
        UiNameEntryStartKeyPress(self, 0x11);
        break;
      case 2:
        self->symbolPage = self->symbolPage == 0;
        UiNameEntryRedrawKeyGrid(self);
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 6, 0, 0);
        }
        UiNameEntryStartKeyPress(self, 0x10);
        break;
      case 3:
        if (strcmp(g_nameEntryKeyTable[0x27], " ") == 0 && self->nameLength == 0) {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 3, 0, 0);
          }
        }
        else {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0, 0, 0);
          }
          len = self->nameLength;
          self->name[len] = 0x27;
          if (len + 1 < 12) {
            self->nameLength = self->nameLength + 1;
          }
          UiNameEntryRedrawName(self);
          UiNameEntryStartKeyPress(self, 10);
        }
        UiNameEntryStartKeyPress(self, 0x10);
        break;
      case 4:
        if (self->name[0] == -1) {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 8, 0, 0);
          }
        }
        else {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 2, 0, 0);
          }
          /* a full name (slot 11 filled) deletes slot 11 without moving back */
          if (self->nameLength > 0 &&
              (self->nameLength != 11 || self->name[self->nameLength] == -1)) {
            self->nameLength = self->nameLength - 1;
          }
          len = self->nameLength;
          self->name[len] = -1;
          for (i = len; i < 11; i++) {
            self->name[i] = self->name[i + 1];
          }
          self->name[11] = -1;
          UiNameEntryRedrawName(self);
        }
        UiNameEntryStartKeyPress(self, 0x10);
        break;
      }
    }
    else if (self->keyRow == 5) {
      if (self->name[0] != -1) {
        self->confirmed = 1;
        self->base.phaseStep = 0;
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 0, 0, 0);
        }
        self->base.phase = 4;
        UiNameEntryStartKeyPress(self, 0x1a);
        sprites = (GfxSprite **)self->base.data;
        sprites[34]->flags &= ~1u;
        sprites = (GfxSprite **)self->base.data;
        sprites[13]->addColor[0] = 0.2f;
        sprites[13]->addColor[1] = 0.2f;
        sprites[13]->addColor[2] = 0.2f;
        sprites[13]->addColor[3] = 1.0f;
        return;
      }
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 8, 0, 0);
      }
    }
    else {
      key = self->page;
      if (self->symbolPage != 0) {
        key = 0xa0;
      }
      key = key + self->keyCol + self->keyRow * 10;
      if (strcmp(g_nameEntryKeyTable[key], " ") == 0 && self->nameLength == 0) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 3, 0, 0);
        }
      }
      else {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 0, 0, 0);
        }
        len = self->nameLength;
        self->name[len] = key;
        if (len + 1 < 12) {
          self->nameLength = self->nameLength + 1;
        }
        UiNameEntryRedrawName(self);
        UiNameEntryStartKeyPress(self, 10);
      }
    }
  }
  else if ((self->base.pad->repeat & 0x20) != 0) {
    if (self->keyRow < 4) {
      self->keyCol = (self->keyCol + 1) % 10;
    }
    else if (self->keyRow == 4) {
      self->keyCol = (self->keyCol + 1) % 5;
    }
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 1, 0, 0);
    }
  }
  else if ((self->base.pad->repeat & 0x80) != 0) {
    if (self->keyRow < 4) {
      self->keyCol = (self->keyCol + 9) % 10;
    }
    else if (self->keyRow == 4) {
      self->keyCol = (self->keyCol + 4) % 5;
    }
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 1, 0, 0);
    }
  }
  else if ((self->base.pad->repeat & 0x40) != 0) {
    if (self->keyRow == 4) {
      self->keyCol = bottomToGrid[self->keyCol];
    }
    self->keyRow = (self->keyRow + 1) % 6;
    if (self->keyRow == 4) {
      self->keyCol = gridToBottom[self->keyCol];
    }
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 1, 0, 0);
    }
  }
  else if ((self->base.pad->repeat & 0x10) != 0) {
    if (self->keyRow == 4) {
      self->keyCol = bottomToGrid[self->keyCol];
    }
    self->keyRow = (self->keyRow + 5) % 6;
    if (self->keyRow == 4) {
      self->keyCol = gridToBottom[self->keyCol];
    }
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 1, 0, 0);
    }
  }

  /* dim sprites 9/12/13/24 on an empty name, 8/23 while nothing is typed */
  sprites = (GfxSprite **)self->base.data;
  if (self->name[0] == -1) {
    sprites[12]->alpha = 0.5f;
    sprites[13]->tint[0] = 0.5f;
    sprites[13]->tint[1] = 0.5f;
    sprites[13]->tint[2] = 0.5f;
    sprites[13]->alpha = 1.0f;
    NameEntryCopyTint(sprites[24], &g_nameEntryColorDimGreen);
    sprites[9]->alpha = 0.5f;
  }
  else {
    sprites[12]->alpha = 1.0f;
    sprites[13]->tint[0] = 1.0f;
    sprites[13]->tint[1] = 1.0f;
    sprites[13]->tint[2] = 1.0f;
    sprites[13]->alpha = 1.0f;
    NameEntryCopyTint(sprites[24], &g_nameEntryColorDarkGreen);
    sprites[9]->alpha = 1.0f;
  }
  if (self->nameLength == 0) {
    NameEntryCopyTint(sprites[23], &g_nameEntryColorDimGreen);
    sprites[8]->alpha = 0.5f;
  }
  else {
    NameEntryCopyTint(sprites[23], &g_nameEntryColorDarkGreen);
    sprites[8]->alpha = 1.0f;
  }
  sprites[13]->textureSlot = 1;

  /* place the cursor sprites for the current row */
  if (self->keyRow < 4) {
    sprites[11]->posX = (float)self->keyCol * 28.0f + 139.0f;
    sprites[11]->posY = (float)self->keyRow * 26.0f + 63.5f;
    sprites[10]->posX = sprites[11]->posX + 21.0f;
    sprites[10]->posY = sprites[11]->posY + 20.0f;
    sprites[10]->flags |= 1;
    sprites[16]->flags &= ~1u;
    sprites[17]->flags &= ~1u;
    sprites[11]->flags |= 1;
    sprites[19]->flags &= ~1u;
    sprites[20]->flags &= ~1u;
    sprites[26]->flags &= ~1u;
    key = self->page;
    if (self->symbolPage != 0) {
      key = 0xa0;
    }
    key = key + self->keyCol + self->keyRow * 10;
    if (strcmp(g_nameEntryKeyTable[key], " ") == 0 && self->nameLength == 0) {
      sprites = (GfxSprite **)self->base.data;
      sprites[11]->flags &= ~1u;
    }
  }
  else if (self->keyRow == 4) {
    if (self->keyCol < 2) {
      sprites[20]->posX = sprites[14 + self->keyCol]->posX - 1.0f;
      x = sprites[14 + self->keyCol]->posX - 2.0f;
      w = GfxSpriteGetWidth(sprites[17]);
      sprites = (GfxSprite **)self->base.data;
      sprites[17]->posX = x + w * 0.5f;
      sprites[16]->flags &= ~1u;
      sprites[17]->flags |= 1;
      sprites[19]->flags &= ~1u;
      sprites[20]->flags |= 1;
    }
    else {
      sprites[19]->posX = sprites[20 + self->keyCol]->posX;
      x = sprites[20 + self->keyCol]->posX;
      w = GfxSpriteGetWidth(sprites[16]);
      sprites = (GfxSprite **)self->base.data;
      sprites[16]->posX = x + w * 0.5f + 0.5f;
      sprites[16]->flags |= 1;
      sprites[17]->flags &= ~1u;
      sprites[19]->flags |= 1;
      sprites[20]->flags &= ~1u;
      /* no space key highlight on an empty name, no delete highlight with nothing typed */
      if ((self->name[0] == -1 && self->keyCol == 3) ||
          (self->nameLength == 0 && self->keyCol == 4)) {
        sprites[19]->flags &= ~1u;
      }
    }
    sprites[10]->flags &= ~1u;
    sprites[11]->flags &= ~1u;
    sprites[26]->flags &= ~1u;
  }
  else if (self->keyRow == 5) {
    sprites[10]->flags &= ~1u;
    sprites[16]->flags &= ~1u;
    sprites[17]->flags &= ~1u;
    sprites[11]->flags &= ~1u;
    sprites[19]->flags &= ~1u;
    sprites[20]->flags &= ~1u;
    sprites[26]->flags |= 1;
    if (self->name[0] != -1) {
      sprites[13]->textureSlot = 0;
    }
  }
  sprites = (GfxSprite **)self->base.data;
  sprites[25]->posX = (float)self->nameLength * 21.0f + 152.5f;
  UiNameEntryAnimateKeyPress(self);

  /* cosine pulse of the cursor colours */
  angle = self->cursorAngle;
  pulseName = (1.0f - __builtin_cosf((float)angle * 0.0055555557f * 3.14159274f)) * 0.5f * 0.5f;
  t = (float)(angle + 60) * 0.0055555557f;
  pulseCursor = (1.0f - __builtin_cosf(t * 3.14159274f)) * 0.5f * 0.8f;
  pulseFrame = (1.0f - __builtin_cosf(t * 3.14159274f)) * 0.5f * 0.1f;
  sprites = (GfxSprite **)self->base.data;
  sprites[25]->addColor[0] = pulseName;
  sprites[25]->addColor[1] = pulseName;
  sprites[25]->addColor[2] = pulseName;
  sprites[25]->addColor[3] = 1.0f;
  sprites[11]->addColor[0] = pulseCursor;
  sprites[11]->addColor[1] = pulseCursor;
  sprites[11]->addColor[2] = pulseCursor;
  sprites[11]->addColor[3] = 1.0f;
  sprites[19]->addColor[0] = pulseCursor;
  sprites[19]->addColor[1] = pulseCursor;
  sprites[19]->addColor[2] = pulseCursor;
  sprites[19]->addColor[3] = 1.0f;
  sprites[20]->addColor[0] = pulseCursor;
  sprites[20]->addColor[1] = pulseCursor;
  sprites[20]->addColor[2] = pulseCursor;
  sprites[20]->addColor[3] = 1.0f;
  sprites[10]->addColor[0] = pulseFrame;
  sprites[10]->addColor[1] = pulseFrame;
  sprites[10]->addColor[2] = pulseFrame;
  sprites[10]->addColor[3] = 1.0f;
  sprites[16]->addColor[0] = pulseFrame;
  sprites[16]->addColor[1] = pulseFrame;
  sprites[16]->addColor[2] = pulseFrame;
  sprites[16]->addColor[3] = 1.0f;
  sprites[17]->addColor[0] = pulseFrame;
  sprites[17]->addColor[1] = pulseFrame;
  sprites[17]->addColor[2] = pulseFrame;
  sprites[17]->addColor[3] = 1.0f;

  /* cosine pulse of the cursor frame scale (1.0 .. 1.05) */
  scale = (1.0f - __builtin_cosf((float)(self->cursorAngle + 60) * 0.0055555557f * 3.14159274f)) *
          0.5f * 0.05f + 1.0f;
  w = GfxSpriteGetWidth(((GfxSprite **)self->base.data)[10]) * scale;
  sprite = ((GfxSprite **)self->base.data)[10];
  h = GfxSpriteGetHeight(sprite) * scale;
  GfxSpriteSetSize(sprite, w, h);
  w = GfxSpriteGetWidth(((GfxSprite **)self->base.data)[16]) * scale;
  sprite = ((GfxSprite **)self->base.data)[16];
  h = GfxSpriteGetHeight(sprite) * scale;
  GfxSpriteSetSize(sprite, w, h);
  w = GfxSpriteGetWidth(((GfxSprite **)self->base.data)[17]) * scale;
  sprite = ((GfxSprite **)self->base.data)[17];
  h = GfxSpriteGetHeight(sprite) * scale;
  GfxSpriteSetSize(sprite, w, h);
  sprites = (GfxSprite **)self->base.data;
  sprites[10]->flags |= 0x20;
  sprites[16]->flags |= 0x20;
  sprites[17]->flags |= 0x20;
  self->cursorAngle = (self->cursorAngle + 20) % 360;
  sprites[13]->addColor[0] = 0.0f;
  sprites[13]->addColor[1] = 0.0f;
  sprites[13]->addColor[2] = 0.0f;
  sprites[13]->addColor[3] = 1.0f;
  sprites[34]->flags &= ~1u;

  /* OK flash: grow and fade sprite 34 (a copy of sprite 26) over 20 frames */
  if (self->keyRow == 5) {
    if (sprites[13]->textureSlot == 0) {
      sprites[13]->addColor[0] = pulseCursor * 0.5f;
      sprites[13]->addColor[1] = pulseCursor * 0.5f;
      sprites[13]->addColor[2] = pulseCursor * 0.5f;
      sprites[13]->addColor[3] = 1.0f;
    }
    sprites[34]->flags |= 1;
    sprite = sprites[34];
    w = GfxSpriteGetWidth(sprites[26]);
    w = w * (self->flashProgress * 0.15f + 1.0f);
    h = GfxSpriteGetHeight(((GfxSprite **)self->base.data)[26]);
    h = h * (self->flashProgress * 0.1f + 1.0f);
    UiSpriteSetSize(w, h, sprite);
    flash = self->flashProgress;
    sprites = (GfxSprite **)self->base.data;
    sprites[34]->addColor[0] = flash;
    sprites[34]->addColor[1] = flash;
    sprites[34]->addColor[2] = flash;
    sprites[34]->addColor[3] = 1.0f;
    sprites[34]->alpha = 1.0f - self->flashProgress;
    flash = self->flashProgress + 0.05f;
    self->flashProgress = flash;
    if (!(flash < 1.0f)) {
      sprites = (GfxSprite **)self->base.data;
      GfxSpriteCopy(sprites[26], sprites[34]);
      self->flashProgress = 0.0f;
    }
  }
  else {
    sprites[34]->flags &= ~1u;
    self->flashProgress = 0.0f;
  }
}
