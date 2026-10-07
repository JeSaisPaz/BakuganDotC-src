// bdc 0x088df214 ActorPlayPlacedMotion
#include "bdc.h"

/* Plays the idle motion configured in the placement record: for character models 0x23..0x57 with a
   record (`+0x350`), picks entry `frozenFlag` (+0x40) of the per-model motion-name list
   `g_actorPlacedMotionNames[model-0x23]` (ended by ""), resolves it (`GmoMotionIndexOfName`) and
   starts it with blend `idleBlend`/60 s (at least 0.2) and loop bit 2 of `idleFlags`, setting
   `motionSlot = 0x32`; unless `force`, does nothing when it already plays. Without a record, or for
   other models, or when the name resolves to no motion, plays motion slot 0 (`ActorPlayMotion`);
   when the entry is past the end of the list, does nothing. */

void ActorPlayPlacedMotion(Actor *self, u8 force)
{
  s32 model;
  const char **names;
  s32 i;
  s32 found;
  s32 index;
  float blend;
  ActorNpcPlacement *rec;

  model = (s32)self->base.base.unk08 - 0x23;
  if (model < 0 || model >= 0x35 || self->placement == NULL) {
    ActorPlayMotion(0.2f, self, 0, 1, 0);
    return;
  }
  names = g_actorPlacedMotionNames[model];
  found = -1;
  for (i = 0; strcmp(names[i], "") != 0; i++) {
    if (((ActorNpcPlacement *)self->placement)->frozenFlag == (u32)i) {
      found = i;
      break;
    }
  }
  if (found == -1) {
    return;
  }
  index = GmoMotionIndexOfName(GmoMotionMgrGet(), names[found]);
  if (index == -1) {
    ActorPlayMotion(0.2f, self, 0, 1, 0);
    return;
  }
  if (GfxModelIsMotion(&self->base, index) && !force) {
    return;
  }
  rec = (ActorNpcPlacement *)self->placement;
  blend = (float)rec->idleBlend * 0.016666668f;
  if (blend < 0.2f) {
    blend = 0.2f;
  }
  GfxModelPlayMotion(blend, &self->base, index, (rec->idleFlags & 4) != 0);
  self->motionSlot = 0x32;
}
