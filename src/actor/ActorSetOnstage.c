// bdc 0x08859954 ActorSetOnstage
#include "bdc.h"

/* Shows or hides the actor by parking it 5000 units away on Y: `onstage == 0` moves `actor+0x24` (Y
   of the position vector at `+0x20`) down by 5000.0 and sets `+0x940 = 1`, `+0x4c1 = 1`; non-zero
   moves it back up by 5000.0, clears both bytes, and calls `ActorCrystalSetMode(actor, 2, 0)`. Stores the
   argument at `+0xa3a` in both cases. After the move it copies the position vector into row 3 of
   the model's root matrix and calls vtable slot +0xb8; the hide path also zeroes `ambient[3]` and `fade`. */

void ActorSetOnstage(Actor *self, char onstage)

{
  ActorCrystal *crystal = (ActorCrystal *)self;
  GfxModel *model = &crystal->base.base;
  GmoModel *gmo;
  const VtblEntry *entry;

  crystal->onstageFlag = onstage;
  if (onstage == '\0') {
    crystal->fadeSkip = 1;
    crystal->base.combat.dead = 1;
    model->pos[1] = model->pos[1] - 5000.0f;
    gmo = model->data;
    gmo->rootMatrix[12] = model->pos[0];
    gmo->rootMatrix[13] = model->pos[1];
    gmo->rootMatrix[14] = model->pos[2];
    gmo->rootMatrix[15] = model->pos[3];
    entry = &((const VtblEntry *)model->base.vtable)[23]; /* +0xb8 */
    ((void (*)(void *))entry->fn)((u8 *)crystal + entry->delta);
    model->ambient[3] = 0.0f;
    crystal->fade = 0.0f;
  } else {
    crystal->scriptedCast = 1;
    crystal->fadeSkip = 0;
    crystal->base.combat.dead = 0;
    model->pos[1] = model->pos[1] + 5000.0f;
    gmo = model->data;
    gmo->rootMatrix[12] = model->pos[0];
    gmo->rootMatrix[13] = model->pos[1];
    gmo->rootMatrix[14] = model->pos[2];
    gmo->rootMatrix[15] = model->pos[3];
    entry = &((const VtblEntry *)model->base.vtable)[23]; /* +0xb8 */
    ((void (*)(void *))entry->fn)((u8 *)crystal + entry->delta);
    crystal->mode2Done = 0;
    ActorCrystalSetMode(crystal, 2, false);
  }
}
