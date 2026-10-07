// bdc 0x08976284 UiCollectionMenuPlayItemBoxMotion
#include "bdc.h"

/* Plays motion `index` of the item-box model of `UiCollectionMenu` (motion
   names at `+0x551 + index*0x40`, frame 0.2, no loop, `GfxModelEnableMotion`/`GfxModelPlayMotionByName`)
   and records it in `+0x753`. */

void UiCollectionMenuPlayItemBoxMotion(UiCollectionMenu *self, u8 index)

{
  GfxModelEnableMotion(self->itemBox);
  GfxModelPlayMotionByName(0.2f,self->itemBox,self->motionNames[index],false);
  self->motion = index;
  return;
}

