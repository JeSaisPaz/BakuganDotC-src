// bdc 0x08935dc0 UiGauntletSetupUpdateAvatarAnim
#include "bdc.h"

/* Drives the avatar model `playerModel` of `UiGauntletSetup` when a push was
   requested (`pushRequested`): first plays `"12_editm_see_gauntlet_push"` once
   (`GfxModelPlayMotionByName`, frame 0.2), calls vtable slot 6 (+0x30) with 0.5 and slot 7 (+0x38, update), and sets
   `pushPlaying`; when that motion has ended (`motionEnded` == 1) switches back to the looping
   `"12_editm_see_gauntlet"` the same way and clears both flags. Does nothing without a model. */

void UiGauntletSetupUpdateAvatarAnim(UiGauntletSetup *self)
{
  GfxModel *model = self->playerModel;
  const VtblEntry *slot;

  if (model == NULL || !self->pushRequested) {
    return;
  }
  if (!self->pushPlaying) {
    GfxModelEnableMotion(model);
    GfxModelPlayMotionByName(0.2f, self->playerModel, "12_editm_see_gauntlet_push", false);
    slot = &((const VtblEntry *)((GfxModel *)self->playerModel)->base.vtable)[6];
    ((void (*)(float, void *))slot->fn)(0.5f, (u8 *)self->playerModel + slot->delta);
    slot = &((const VtblEntry *)((GfxModel *)self->playerModel)->base.vtable)[7];
    ((void (*)(void *))slot->fn)((u8 *)self->playerModel + slot->delta);
    self->pushPlaying = 1;
  } else if (model->motionEnded == 1) {
    GfxModelEnableMotion(model);
    GfxModelPlayMotionByName(0.2f, self->playerModel, "12_editm_see_gauntlet", true);
    slot = &((const VtblEntry *)((GfxModel *)self->playerModel)->base.vtable)[6];
    ((void (*)(float, void *))slot->fn)(0.5f, (u8 *)self->playerModel + slot->delta);
    slot = &((const VtblEntry *)((GfxModel *)self->playerModel)->base.vtable)[7];
    ((void (*)(void *))slot->fn)((u8 *)self->playerModel + slot->delta);
    self->pushRequested = 0;
    self->pushPlaying = 0;
  }
}
