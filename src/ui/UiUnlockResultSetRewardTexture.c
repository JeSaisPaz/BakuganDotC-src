// bdc 0x08939a24 UiUnlockResultSetRewardTexture
#include "bdc.h"

/* Points `sprite`'s texture at the image of the reward of `UiUnlockResult` by
   reward kind `+0x5ee`: 0/1 `"ga_card_L_%03d"` (card `+0x7ec` + 1), 2 `"repair_card_%03d"`, 3/7
   `"tuukou_card01"`, 4/8/9 and others `"NONE"`; kinds 5/6 (3D models) leave it unchanged. */

void UiUnlockResultSetRewardTexture(UiUnlockResult *self, GfxSprite *sprite)

{
  char name[64];
  
  switch(self->rewardKind) {
  case '\0':
  case '\x01':
    sprintf(name,"ga_card_L_%03d",self->rewardIndex + 1);
    break;
  case '\x02':
    sprintf(name,"repair_card_%03d",self->rewardIndex);
    break;
  case '\x03':
    sprintf(name,"tuukou_card%02d",1);
    break;
  case '\x04':
  case '\b':
  case '\t':
    sprintf(name,"NONE");
    break;
  case '\x05':
  case '\x06':
    return;
  case '\a':
    sprintf(name,"tuukou_card%02d",1);
    break;
  default:
    sprintf(name,"NONE");
  }
  sprite->texture = GfxFindTexture(name);
  return;
}

