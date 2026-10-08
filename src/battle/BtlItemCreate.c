// bdc 0x088b6584 BtlItemCreate
#include "bdc.h"

/* Allocates a 0x80-byte battle item from the low end of the heap (allocation policy saved
   and restored around `MemAlloc`, under `MemLock`) and constructs it
   (`BtlItemCtor``(item, kind, pos)`), then sets its `appearAnim`, `lifetime` and `spawner`.
   The three stores are not guarded: when the allocation fails they write through NULL. */

void BtlItemCreate(s32 kind, u32 *pos, u32 life, u8 appearAnim, void *spawner)
{
    bool fromLow;
    BtlItem *mem;
    BtlItem *item;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = (BtlItem *)MemAlloc(sizeof(BtlItem), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    item = NULL;
    if (mem != NULL) {
        BtlItemCtor(mem, kind, (float *)pos);
        item = mem;
    }
    item->appearAnim = appearAnim;
    item->lifetime = (s32)life;
    item->spawner = spawner;
}
