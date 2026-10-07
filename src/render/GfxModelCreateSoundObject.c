// bdc 0x089de894 GfxModelCreateSoundObject
#include "bdc.h"

/* Creates a sound object with `slotCount` slots in the sound-object list (`SndGetObjectList` /
   `SndObjectCreate`), stores it in the model at `+0x12c` and makes it follow the model's position
   (`SndObjectSetPosPtr(obj, model + 0x20)`). `BtlBakuganCtor` calls it with 8 slots. */

void GfxModelCreateSoundObject(GfxModel *self, s32 slotCount)

{
  CoreNodeOwner *owner;
  SndObject *obj;
  
  owner = SndGetObjectList();
  obj = (SndObject *)SndObjectCreate(owner,slotCount);
  self->sound = obj;
  SndObjectSetPosPtr(obj,self->pos);
  return;
}

