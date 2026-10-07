// bdc 0x088d76d8 GameGimmickItemBoxActivate
#include "bdc.h"

/* Virtual slot 16 (`+0x84`) of the item box (breakable obstacle) gimmick
   (`GameGimmickItemBoxCtor`, vtables `0x08af30f4`/`0x08af319c`), called by the field's activate
   helpers (`GameFieldActivateItemBox`): deactivates it (`+0x15e = 0`) and with `mode == 1` jumps
   its motion to the end (`GfxModelSetMotionProgress`, 1.0) and clears the break alpha. */

void GameGimmickItemBoxActivate(GameGimmickItemBox *obj, s8 mode)

{
  if (((obj->base).active != '\0') && ((obj->base).active = '\0', mode == '\x01')) {
    GfxModelSetMotionProgress((GfxModel *)obj,1.0f);
    obj->breakAlpha = 0.0f;
  }
  return;
}

