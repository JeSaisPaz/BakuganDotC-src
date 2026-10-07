// bdc 0x088d76d0 GameGimmickItemBoxGetState
#include "bdc.h"

/* Returns the state of the item box (breakable obstacle) gimmick
   (`GameGimmickItemBoxCtor`, vtables `0x08af30f4`/`0x08af319c`). */
s32 GameGimmickItemBoxGetState(GameGimmick *self)
{
    return self->state;
}
