// bdc 0x089ec878 UiMsgBoxUpdate
#include "bdc.h"

/* Per-frame state machine of the message box, run by `UiTextTaskUpdate`. Reads the pressed pad
   buttons (`*unk04`, else `g_padState``->pressed`) and runs state `id`:
   0 opens: deletes an old owner window, creates a new one (`UiWindowFrameCreate`) on `extents`
     (with the blinking `"push_button_s"` prompt sprite `unk50` when the box waits for a button and
     has no choices), starts its open animation (vtable slot 4, 8/60 s), plays sound 1 and makes the
     text opaque;
   1 waits for the window to finish opening (`state == 0`), then shows the prompt;
   2 shows the text (`UiMsgBoxPrintText`, `UiMsgBoxUpdateCursor`): with choices, cross cancels
     (`choice = -1`, close), circle confirms (closes after half a second via `unk1c`), up/down or
     left/right (layout `unk70`) move the clamped choice (sound 1, cursor fade reset); without
     choices, the prompt animates (`g_uiMsgBoxPromptAnim`) and any face button closes
     (`UiMsgBoxRequestClose`, sound 0); a positive `unk1c` counts down to an automatic close, -2
     stays open. On a close request it releases the prompt (`UiSpriteLayerRelease`), starts the
     close animation (vtable slot 5, 10/60 s), halves the text alpha and hides the highlight;
   3 fades the text and, once the window is closed, deletes it and frees the text buffer;
   other states (< 0, >= 4) clear the close request and reset the depth `unk10` to `unk14`.
   Finally the owner window, if any, gets its update call (vtable slot 2). */

/* Copies a quad (lv.q/sv.q C000 in the listing; C000 is not read after the copy). */
static inline void QuadCopy(float *dst, const float *src)
{
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

void UiMsgBoxUpdate(UiMsgBox *self)
{
  u16 pressed;
  const VtblEntry *entry;
  GfxSprite *sprite;
  float *color;
  void *text;
  s32 prev;
  s8 cell;
  float frame;
  float rect[4] __attribute__((aligned(16)));
  float uv[4];
  float uv2[4];
  float rect2[4] __attribute__((aligned(16)));

  if (self->unk04 != NULL) {
    pressed = *self->unk04;
  } else {
    pressed = g_padState->pressed;
  }

  switch (self->id) {
  case 0:
    if (self->owner != NULL) {
      entry = &((GfxSpriteLayer *)self->owner)->vtbl[1];
      ((void (*)(void *, int))entry->fn)((char *)self->owner + entry->delta, 3);
      self->owner = NULL;
    }
    if (self->unk1c == -1 && self->choices == NULL) {
      QuadCopy(rect, self->extents);
      self->owner = UiWindowFrameCreate(self->state, rect, 0.0f);
      rect[0] = 448.0f;
      rect[1] = 232.0f;
      rect[2] = 0.0f;
      rect[3] = 0.0f;
      sprite = GfxSpriteLayerCreateSpriteByName((GfxSpriteLayer *)self->owner, "push_button_s", rect,
                                                false);
      self->unk50 = sprite;
      UiSpriteSetSize(24.0f, 32.0f, sprite);
      uv[0] = 0.0f;
      uv[1] = 0.0f;
      uv[2] = 24.0f;
      uv[3] = 32.0f;
      GfxSpriteSetUvRectXYWH(self->unk50, uv);
      self->unk50->flags &= ~1u;
    } else {
      QuadCopy(rect2, self->extents);
      self->owner = UiWindowFrameCreate(self->state, rect2, 0.0f);
    }
    {
      UiWindowFrame *owner = self->owner;
      entry = &((GfxSpriteLayer *)owner)->vtbl[4];
      ((void (*)(void *, int, bool))entry->fn)((char *)owner + entry->delta,
                                               (GfxDisplayGetFps(g_gfxDisplay) * 8) / 60,
                                               self->unk78);
    }
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 1, 0, 0);
    }
    UiTextBoxGetColor(self->textBox)[3] = 1.0f;
    self->id++;
    break;

  case 1:
    if (self->owner->state == 0) {
      if (self->unk50 != NULL) {
        self->unk50->flags |= 1;
      }
      self->id++;
    }
    break;

  case 2:
    if (self->unk1c == -1) {
      if (self->choices != NULL) {
        if (pressed & 0x4000) { /* cross: cancel */
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 2, 0, 0);
          }
          self->choice = -1;
          self->unk59 = 1;
        } else if (pressed & 0x2000) { /* circle: confirm, close after half a second */
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 2, 0, 0);
          }
          self->unk1c = GfxDisplayGetFps(g_gfxDisplay) / 2;
        } else {
          prev = self->choice;
          switch (self->unk70) {
          case 1: /* vertical: up / down */
            if (pressed & 0x10) {
              self->choice--;
            } else if (pressed & 0x40) {
              self->choice++;
            }
            break;
          case 2: /* horizontal: left / right */
            if (pressed & 0x80) {
              self->choice--;
            } else if (pressed & 0x20) {
              self->choice++;
            }
            break;
          }
          if (self->choice < 0) {
            self->choice = 0;
          } else if (self->choice >= self->unk64) {
            self->choice = self->unk64 - 1;
          }
          if (prev != self->choice) {
            self->unk68 = 1.0f;
            self->unk6c = -(1.8f / (float)GfxDisplayGetFps(g_gfxDisplay));
            if (SndHasManager()) {
              SndManagerPlay(SndGetManager(), 1, 0, 0);
            }
          }
        }
      } else if (self->unk50 != NULL) {
        cell = g_uiMsgBoxPromptAnim[(s32)self->unk54];
        uv2[0] = g_uiMsgBoxPromptCellX[cell & 1];
        uv2[1] = g_uiMsgBoxPromptCellY[(cell >> 1) & 1];
        uv2[2] = 24.0f;
        uv2[3] = 32.0f;
        GfxSpriteSetUvRectXYWH(self->unk50, uv2);
        frame = self->unk54 + (float)(g_gfxDisplay->frameSkip + 1) * 0.5f;
        self->unk54 = frame;
        if ((s32)frame >= 13) {
          self->unk54 = 0.0f;
        }
        if (pressed & 0xf000) {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0, 0, 0);
          }
          UiMsgBoxRequestClose(self);
        }
      }
    } else if (self->unk1c != -2) {
      if (self->unk1c > 0) {
        self->unk1c--;
      } else {
        self->unk59 = 1;
      }
    }
    if (self->unk59 != 0) {
      if (self->unk50 != NULL) {
        UiSpriteLayerRelease(self->owner, self->unk50);
        self->unk50 = NULL;
      }
      {
        UiWindowFrame *owner = self->owner;
        entry = &((GfxSpriteLayer *)owner)->vtbl[5];
        ((void (*)(void *, int, bool))entry->fn)((char *)owner + entry->delta,
                                                 (GfxDisplayGetFps(g_gfxDisplay) * 10) / 60, true);
      }
      color = UiTextBoxGetColor(self->textBox);
      color[3] = color[3] * 0.5f;
      GfxRectSetVisible(self->highlight, 0);
      self->id++;
    }
    UiMsgBoxPrintText(self);
    UiMsgBoxUpdateCursor(self);
    break;

  case 3:
    GfxRectSetVisible(self->highlight, 0);
    color = UiTextBoxGetColor(self->textBox);
    color[3] = color[3] * 0.1f;
    UiMsgBoxPrintText(self);
    if (self->owner->state == 0) {
      if (self->owner != NULL) {
        entry = &((GfxSpriteLayer *)self->owner)->vtbl[1];
        ((void (*)(void *, int))entry->fn)((char *)self->owner + entry->delta, 3);
        self->owner = NULL;
      }
      if (self->text != NULL) {
        text = self->text;
        MemLock();
        MemFree(text, NULL, 0);
        MemUnlock();
        self->text = NULL;
      }
      self->id++;
    }
    break;

  default:
    self->unk59 = 0;
    self->unk10 = self->unk14;
    break;
  }

  if (self->owner != NULL) {
    entry = &((GfxSpriteLayer *)self->owner)->vtbl[2];
    ((void (*)(void *))entry->fn)((char *)self->owner + entry->delta);
  }
}
