// bdc 0x08856354 ActorCrystalSpawnAtPoint
#include "bdc.h"

/* Spawns a crystal actor at crystal spawn point `index`: takes the point's position
   (`ActorStageObjRecordGetType4Pos`), raises it by 100 and drops it onto the ground
   (`CollisionRaycastPoint`), allocates 0xa90 bytes from the low end of the heap under the heap
   lock (`MemAlloc`) and builds an `ActorCrystal` there (`ActorCrystalCtor`, model id 0x21,
   mode 2, flag/type/style 0). Stores `index` in `spawnPoint`, sets `fireType` to 0 when
   `SaveGetProfileFlag0` is set and 2 otherwise, calls virtual entry 7 (`+0x38`,
   `ActorCrystalUpdate` in the crystal vtable) and returns the crystal. A failed allocation is
   not handled: the stores and the virtual call then go through NULL. Used by
   `ActorCrystalSpawnTaskStep`. The constructor gets a copy of the grounded position (all four
   lanes); the C000 the copy leaves loaded at the virtual call is not read by
   `ActorCrystalUpdate`, so it is no hand-off. */

void *ActorCrystalSpawnAtPoint(int index)
{
    float pos[4];
    float ctorPos[4];
    ActorCrystal *mem;
    ActorCrystal *crystal;
    const VtblEntry *vtbl;
    bool fromLow;

    ActorStageObjRecordGetType4Pos(pos, index);
    pos[1] = pos[1] + 100.0f;
    CollisionRaycastPoint(pos, pos);
    crystal = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = (ActorCrystal *)MemAlloc(0xa90, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
        ctorPos[0] = pos[0];
        ctorPos[1] = pos[1];
        ctorPos[2] = pos[2];
        ctorPos[3] = pos[3];
        ActorCrystalCtor(mem, 0x21, 2, ctorPos, 0, 0, 0);
        crystal = mem;
    }
    crystal->spawnPoint = (u32)index;
    if (SaveGetProfileFlag0() == 0) {
        crystal->fireType = 2;
    } else {
        crystal->fireType = 0;
    }
    vtbl = &((const VtblEntry *)crystal->base.base.base.vtable)[7];
    ((void (*)(void *))vtbl->fn)((u8 *)crystal + vtbl->delta);
    return crystal;
}
