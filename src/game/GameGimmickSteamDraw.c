// bdc 0x088d5fcc GameGimmickSteamDraw
#include "bdc.h"

/* Draw (vtable slot 8) of the steam gimmick (`GameGimmickSteamCtor`, vtables
   `0x08af2edc`/`0x08af2f7c`): empty (the steam is drawn by the effect). */

void GameGimmickSteamDraw(CoreObject *obj, u32 **dl)
{
    (void)obj;
    (void)dl;
}
