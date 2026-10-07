// bdc 0x089de8e0 GfxModelReleaseSound
#include "bdc.h"

/* Releases the 3D sound object attached to a model (`+0x12c`) back to the sound object list
   (`SndGetObjectList`, `SndObjectRelease`); called from `BtlBakuganDtor`, `ActorDtor` and
   `GameGimmickCameraDtor`. */

void GfxModelReleaseSound(GfxModel *self)

{
  CoreNodeOwner *owner;
  
  owner = SndGetObjectList();
  SndObjectRelease(owner,&self->sound->node);
  return;
}

