// bdc 0x088d7304 GameGimmickItemBoxUpdatePrompt
#include "bdc.h"

/* While the item box (breakable obstacle) gimmick (`GameGimmickItemBoxCtor`, vtables
   `0x08af30f4`/`0x08af319c`) is active (`+0x15e`), sets the prompt flag (`+0x15f` bit 1) when the
   player is within 21 units (`GameIsPlayerWithinRange`) and in front
   (`GameGetPlayerFacingSectorFrom` = 1). */

void GameGimmickItemBoxUpdatePrompt(GameGimmickItemBox *obj)
{
  float a[4];
  float b[4];

  if (obj->base.active != 0) {
    obj->base.contactFlags &= 0xfd;
    if (ActorFindPlayer() != 0) {
      a[0] = obj->base.base.pos[0];
      a[1] = obj->base.base.pos[1];
      a[2] = obj->base.base.pos[2];
      a[3] = obj->base.base.pos[3];
      if (GameIsPlayerWithinRange(21.0f, obj, a) == 1) {
        b[0] = obj->base.base.pos[0];
        b[1] = obj->base.base.pos[1];
        b[2] = obj->base.base.pos[2];
        b[3] = obj->base.base.pos[3];
        if (GameGetPlayerFacingSectorFrom(obj, b) == 1) {
          obj->base.contactFlags |= 2;
        }
      }
    }
  }
}
