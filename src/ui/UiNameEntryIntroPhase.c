// bdc 0x08807d18 UiNameEntryIntroPhase
#include "bdc.h"

/* Phase 2 of the name entry screen (`UiNameEntry`), stepped by `phaseStep`:
   step 0: slides the avatar left by 12 per frame (ticking the intro sound); once its x is <= 0
   plays avatar motion 0 (looping) and goes to step 1.
   step 1: turns the avatar and the base matrix by -0.0942 rad per frame about Y (the matrix keeps
   its translation row); once the avatar's Y rotation is <= pi/2 shows sprites 1, 21, 27, 28, 32, 33
   and goes to step 2.
   step 2: fades those sprites in over 6 frames (`repeatTimer`/6); then shows sprites 2-9, 12-15,
   18, 22-24, 32, 33, 37-42, resets the timer and goes to step 3.
   step 3: fades the key-grid sprites in over 6 frames (some at half alpha / half tint); then shows
   sprites 10, 11 and 25 at full alpha (25 tinted with `g_nameEntryColorHighlight`), redraws the
   key grid (`UiNameEntryRedrawKeyGrid`), resets timer and `phaseStep` and switches to phase 3.
   Negative or larger `phaseStep` values do nothing. */

void UiNameEntryIntroPhase(UiNameEntry *self)
{
  GfxModel *avatar;
  GfxSprite **sprites;
  GfxSprite *s;
  float alpha;
  float half;
  int i;
  float *m;
  float c;
  float sn;
  float x;
  float z;

  switch (self->base.phaseStep) {
  case 0:
    avatar = (GfxModel *)self->avatar;
    avatar->pos[0] = avatar->pos[0] - 12.0f;
    UiNameEntryTickIntroSound(self);
    if (((GfxModel *)self->avatar)->pos[0] <= 0.0f) {
      UiNameEntryPlayAvatarMotion(self, 0, 1);
      self->base.phaseStep = 1;
    }
    break;
  case 1:
    avatar = (GfxModel *)self->avatar;
    avatar->rot[1] = avatar->rot[1] - 0.09424778f;
    /* vrot of angle * 2/pi (S703): R rows (c, 0, -s, 0), (0, 1, 0, 0), (s, 0, c, 0), (0, 0, 0, 1);
       each baseMatrix row becomes row * R (vmmul.q E200, E100, E000), then the translation row
       (+0x30), saved before, is stored back, so only rows 0..2 change. */
    m = self->baseMatrix;
    c = __builtin_cosf(0.09424778f); /* 0x3dc104fb */
    sn = __builtin_sinf(0.09424778f);
    for (i = 0; i < 3; i++) {
      x = m[i * 4 + 0];
      z = m[i * 4 + 2];
      m[i * 4 + 0] = x * c + z * sn;
      m[i * 4 + 2] = -(x * sn) + z * c;
    }
    if (((GfxModel *)self->avatar)->rot[1] <= 1.5707964f) {
      ((GfxSprite **)self->base.data)[1]->flags |= 1;
      ((GfxSprite **)self->base.data)[21]->flags |= 1;
      ((GfxSprite **)self->base.data)[27]->flags |= 1;
      ((GfxSprite **)self->base.data)[28]->flags |= 1;
      ((GfxSprite **)self->base.data)[32]->flags |= 1;
      ((GfxSprite **)self->base.data)[33]->flags |= 1;
      self->base.phaseStep = 2;
    }
    break;
  case 2:
    self->repeatTimer++;
    alpha = (float)self->repeatTimer * 0.16666667f;
    ((GfxSprite **)self->base.data)[33]->alpha = alpha;
    ((GfxSprite **)self->base.data)[32]->alpha = alpha;
    ((GfxSprite **)self->base.data)[28]->alpha = alpha;
    ((GfxSprite **)self->base.data)[27]->alpha = alpha;
    ((GfxSprite **)self->base.data)[21]->alpha = alpha;
    ((GfxSprite **)self->base.data)[1]->alpha = alpha;
    if (self->repeatTimer >= 6) {
      for (i = 2; i < 10; i++) {
        ((GfxSprite **)self->base.data)[i]->flags |= 1;
      }
      ((GfxSprite **)self->base.data)[12]->flags |= 1;
      ((GfxSprite **)self->base.data)[13]->flags |= 1;
      ((GfxSprite **)self->base.data)[14]->flags |= 1;
      ((GfxSprite **)self->base.data)[15]->flags |= 1;
      ((GfxSprite **)self->base.data)[18]->flags |= 1;
      ((GfxSprite **)self->base.data)[22]->flags |= 1;
      ((GfxSprite **)self->base.data)[23]->flags |= 1;
      ((GfxSprite **)self->base.data)[24]->flags |= 1;
      ((GfxSprite **)self->base.data)[32]->flags |= 1;
      ((GfxSprite **)self->base.data)[33]->flags |= 1;
      for (i = 37; i < 43; i++) {
        ((GfxSprite **)self->base.data)[i]->flags |= 1;
      }
      self->repeatTimer = 0;
      self->base.phaseStep = 3;
    }
    break;
  case 3:
    self->repeatTimer++;
    alpha = (float)self->repeatTimer * 0.16666667f;
    for (i = 2; i < 8; i++) {
      ((GfxSprite **)self->base.data)[i]->alpha = alpha;
    }
    ((GfxSprite **)self->base.data)[24]->alpha = alpha;
    ((GfxSprite **)self->base.data)[23]->alpha = alpha;
    ((GfxSprite **)self->base.data)[22]->alpha = alpha;
    ((GfxSprite **)self->base.data)[18]->alpha = alpha;
    ((GfxSprite **)self->base.data)[15]->alpha = alpha;
    half = alpha * 0.5f;
    ((GfxSprite **)self->base.data)[14]->alpha = alpha;
    s = ((GfxSprite **)self->base.data)[13];
    s->tint[0] = half;
    s->tint[1] = half;
    s->tint[2] = half;
    s->alpha = 1.0f;
    ((GfxSprite **)self->base.data)[9]->alpha = half;
    ((GfxSprite **)self->base.data)[8]->alpha = half;
    ((GfxSprite **)self->base.data)[12]->alpha = half;
    for (i = 37; i < 43; i++) {
      ((GfxSprite **)self->base.data)[i]->alpha = alpha;
    }
    if (self->repeatTimer >= 6) {
      ((GfxSprite **)self->base.data)[10]->flags |= 1;
      ((GfxSprite **)self->base.data)[10]->alpha = 1.0f;
      ((GfxSprite **)self->base.data)[11]->flags |= 1;
      ((GfxSprite **)self->base.data)[20]->alpha = 1.0f;
      ((GfxSprite **)self->base.data)[19]->alpha = 1.0f;
      ((GfxSprite **)self->base.data)[11]->alpha = 1.0f;
      ((GfxSprite **)self->base.data)[16]->alpha = 1.0f;
      ((GfxSprite **)self->base.data)[17]->alpha = 1.0f;
      ((GfxSprite **)self->base.data)[25]->flags |= 1;
      sprites = (GfxSprite **)self->base.data;
      /* lv.q/sv.q 16-byte copy: tint[0..2] and alpha */
      sprites[25]->tint[0] = g_nameEntryColorHighlight.x;
      sprites[25]->tint[1] = g_nameEntryColorHighlight.y;
      sprites[25]->tint[2] = g_nameEntryColorHighlight.z;
      sprites[25]->alpha = g_nameEntryColorHighlight.w;
      ((GfxSprite **)self->base.data)[26]->alpha = 1.0f;
      UiNameEntryRedrawKeyGrid(self);
      self->repeatTimer = 0;
      self->base.phaseStep = 0;
      self->base.phase = 3;
    }
    break;
  }
}
