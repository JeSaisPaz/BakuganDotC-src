// bdc 0x0889fd80 GameGimmickCorePointSetBuffer
#include "bdc.h"

/* Setter: stores `buf` at `+0x2d4` of a core-point gimmick; the destructor
   `GameGimmickCorePointDtor` releases it with `MemFreeAligned`. */
void GameGimmickCorePointSetBuffer(GameGimmickCorePoint *obj, void *buf)
{
    obj->buffer = buf;
}
