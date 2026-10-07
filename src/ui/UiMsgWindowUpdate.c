// bdc 0x08816eb8 UiMsgWindowUpdate
#include "bdc.h"

/* Per-frame update of the message window (`UiMsgWindowCtor`) (called by the text render task
   `UiTextTaskUpdate`), a state machine on `state`: 0 fades `alpha` in over 1/6 s, then plays
   sound 1 and sets `unk1d`; 1 counts down `delay` (or advances at once on `closeRequested`) and,
   once the delay is -1, reads the pad (cached in `pad` from `g_padState`): in yes/no mode
   (`mode == 0`) left/right (pressed 0x80/0x20) move `choice` between 0 and 1 (restarting the
   pulse, sound 1), confirm (0x4000, sound 0) sets `hasChoice` and closes after half a second,
   cancel (0x2000, sound 2) sets choice -1, `hasChoice` and advances; in OK mode (`mode == 1`)
   only confirm counts; `text` is printed each frame at (`textX`, `textY` - 12) with
   `UiTextBoxPrint`. 2 fades out, hides `rect` and the 11 frame sprites (no button update that
   frame). Negative or closed (> 2) states clear `closeRequested`. In states 0/1 it then pulses the
   highlighted button (yes/no buttons with the `co_bo_ita_1_01` / `co_bo_ita_2_01` textures, or
   the OK button), and in every state copies the window alpha to all 11 frame sprites.
   The cleared add colour of the yes/no button sprites is the VFPU bank constant C720 = (0, 0, 0, 0). */

void UiMsgWindowUpdate(UiMsgWindow *self)
{
  s32 state;
  s32 delay;
  s32 dir;
  s32 choice;
  s32 i;
  bool active;
  float alpha;
  float step;
  float pulse;
  float level;
  float half;
  void *texOn;
  void *texOff;
  PadState *pad;
  GfxSprite *spr;

  state = self->state;
  active = false;
  if (state > 0) {
    if (state < 2) {
      /* state 1: wait for input */
      delay = self->delay;
      if (self->closeRequested != 0) {
        self->state = state + 1;
      } else if (delay > 0) {
        delay = delay - 1;
        self->delay = delay;
      } else if (delay >= 0) {
        self->state = state + 1;
      }
      if (delay == -1) {
        if (self->pad == NULL) {
          self->pad = g_padState;
        }
        if (self->mode == 0) {
          pad = self->pad;
          if ((pad->pressed & 0x2000) != 0) {
            if (SndHasManager()) {
              SndManagerPlay(SndGetManager(), 2, 0, 0);
            }
            self->choice = -1;
            self->state = self->state + 1;
            self->hasChoice = 1;
          } else if ((pad->pressed & 0x4000) != 0) {
            if (SndHasManager()) {
              SndManagerPlay(SndGetManager(), 0, 0, 0);
            }
            self->delay = GfxDisplayGetFps(g_gfxDisplay) / 2;
            self->hasChoice = 1;
          } else {
            dir = 0;
            if ((pad->pressed & 0x80) != 0) {
              dir = -1;
            } else if ((pad->pressed & 0x20) != 0) {
              dir = 1;
            }
            if (dir == -1) {
              self->choice = self->choice - 1;
              if (self->choice < 0) {
                self->choice = 0;
              } else {
                self->pulse = 1.0f;
                self->pulseStep = -(3.0f / (float)GfxDisplayGetFps(g_gfxDisplay));
                if (SndHasManager()) {
                  SndManagerPlay(SndGetManager(), 1, 0, 0);
                }
              }
            } else if (dir == 1) {
              self->choice = self->choice + 1;
              if (self->choice >= 2) {
                self->choice = 1;
              } else {
                self->pulse = 1.0f;
                self->pulseStep = -(3.0f / (float)GfxDisplayGetFps(g_gfxDisplay));
                if (SndHasManager()) {
                  SndManagerPlay(SndGetManager(), 1, 0, 0);
                }
              }
            }
          }
        } else if (self->mode == 1) {
          if ((self->pad->pressed & 0x4000) != 0) {
            if (SndHasManager()) {
              SndManagerPlay(SndGetManager(), 0, 0, 0);
            }
            self->delay = GfxDisplayGetFps(g_gfxDisplay) / 2;
            self->hasChoice = 1;
          }
        }
      }
      if (self->text != NULL) {
        UiTextBoxPrint(self->textBox, self->textX, self->textY - 12, self->text, 1, 1);
      }
      active = true;
    } else if (state < 3) {
      /* state 2: fade out, then hide everything */
      alpha = self->alpha;
      if (!(alpha <= 0.0f)) {
        alpha = alpha - 6.0f / (float)GfxDisplayGetFps(g_gfxDisplay);
        self->alpha = alpha;
      }
      if (alpha <= 0.0f) {
        self->alpha = 0.0f;
        self->state = self->state + 1;
        GfxRectSetVisible(self->rect, 0);
        for (i = 0; i < 11; i++) {
          self->sprites[i]->flags &= ~1u;
        }
      }
    } else {
      self->closeRequested = 0;
    }
  } else if (state >= 0) {
    /* state 0: fade in */
    alpha = self->alpha;
    if (alpha < 1.0f) {
      alpha = alpha + 6.0f / (float)GfxDisplayGetFps(g_gfxDisplay);
      self->alpha = alpha;
    }
    if (!(alpha < 1.0f)) {
      self->alpha = 1.0f;
      self->state = self->state + 1;
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      self->unk1d = 1;
    }
    active = true;
  } else {
    self->closeRequested = 0;
  }

  if (active) {
    level = 0.0f;
    if (self->choice >= 0) {
      step = self->pulseStep;
      pulse = self->pulse + step;
      self->pulse = pulse;
      if (self->delay > 0) {
        pulse = pulse + step * 12.0f;
        self->pulse = pulse;
      }
      if (!(pulse <= 1.0f)) {
        self->pulse = 1.0f;
        self->pulseStep = -step;
        pulse = 1.0f;
      } else if (pulse < 0.0f) {
        self->pulse = 0.0f;
        self->pulseStep = -step;
        pulse = 0.0f;
      }
      level = pulse * 0.5f + 0.5f;
    }
    if (self->mode == 0) {
      texOn = GfxFindTexture("co_bo_ita_1_01");
      texOff = GfxFindTexture("co_bo_ita_2_01");
      self->sprites[4]->flags &= ~1u;
      spr = self->sprites[4];
      spr->addColor[0] = 0.0f;
      spr->addColor[1] = 0.0f;
      spr->addColor[2] = 0.0f;
      spr->addColor[3] = 0.0f;
      self->sprites[7]->flags &= ~1u;
      spr = self->sprites[7];
      spr->addColor[0] = 0.0f;
      spr->addColor[1] = 0.0f;
      spr->addColor[2] = 0.0f;
      spr->addColor[3] = 0.0f;
      spr = self->sprites[4];
      spr->tint[0] = 1.0f;
      spr->tint[1] = 1.0f;
      spr->tint[2] = 1.0f;
      spr->alpha = 1.0f;
      spr = self->sprites[3];
      spr->addColor[0] = 0.0f;
      spr->addColor[1] = 0.0f;
      spr->addColor[2] = 0.0f;
      spr->addColor[3] = 0.0f;
      spr = self->sprites[3];
      spr->tint[0] = 1.0f;
      spr->tint[1] = 1.0f;
      spr->tint[2] = 1.0f;
      spr->alpha = 1.0f;
      self->sprites[3]->texture = texOff;
      spr = self->sprites[7];
      spr->tint[0] = 1.0f;
      spr->tint[1] = 1.0f;
      spr->tint[2] = 1.0f;
      spr->alpha = 1.0f;
      spr = self->sprites[6];
      spr->addColor[0] = 0.0f;
      spr->addColor[1] = 0.0f;
      spr->addColor[2] = 0.0f;
      spr->addColor[3] = 0.0f;
      spr = self->sprites[6];
      spr->tint[0] = 1.0f;
      spr->tint[1] = 1.0f;
      spr->tint[2] = 1.0f;
      spr->alpha = 1.0f;
      self->sprites[6]->texture = texOff;
      choice = self->choice;
      if (choice > 0) {
        if (choice < 2) {
          /* "no" highlighted */
          self->sprites[6]->texture = texOn;
          spr = self->sprites[6];
          spr->tint[0] = level;
          spr->tint[1] = level;
          spr->tint[2] = level;
          spr->alpha = 1.0f;
          self->sprites[4]->flags &= ~1u;
          half = level * 0.5f;
          self->sprites[7]->flags |= 1;
          spr = self->sprites[6];
          spr->addColor[0] = half;
          spr->addColor[1] = half;
          spr->addColor[2] = 0.0f;
          spr->addColor[3] = 1.0f;
          spr = self->sprites[7];
          spr->addColor[0] = half;
          spr->addColor[1] = half;
          spr->addColor[2] = 0.0f;
          spr->addColor[3] = 1.0f;
        }
      } else if (choice >= 0) {
        /* "yes" highlighted */
        self->sprites[3]->texture = texOn;
        spr = self->sprites[3];
        spr->tint[0] = level;
        spr->tint[1] = level;
        spr->tint[2] = level;
        spr->alpha = 1.0f;
        self->sprites[4]->flags |= 1;
        half = level * 0.5f;
        self->sprites[7]->flags &= ~1u;
        spr = self->sprites[3];
        spr->addColor[0] = half;
        spr->addColor[1] = half;
        spr->addColor[2] = 0.0f;
        spr->addColor[3] = 1.0f;
        spr = self->sprites[4];
        spr->addColor[0] = half;
        spr->addColor[1] = half;
        spr->addColor[2] = 0.0f;
        spr->addColor[3] = 1.0f;
      }
    } else if (self->mode == 1) {
      spr = self->sprites[8];
      spr->tint[0] = level;
      spr->tint[1] = level;
      spr->tint[2] = level;
      spr->alpha = 1.0f;
      half = level * 0.5f;
      spr = self->sprites[10];
      spr->addColor[2] = 0.0f;
      spr->addColor[0] = half;
      spr->addColor[1] = half;
      spr->addColor[3] = 1.0f;
    }
  }

  for (i = 0; i < 11; i++) {
    self->sprites[i]->alpha = self->alpha;
  }
}
