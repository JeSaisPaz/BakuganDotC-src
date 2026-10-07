// bdc 0x0885646c ActorCrystalStandSpawnAtPoint
#include "bdc.h"

/* Creates the crystal pedestal model (0x150 bytes, `ActorCrystalStandCtor`
   `"fz_crystal01_stand_60.gmo"`) at crystal spawn point `index` dropped onto the ground
   (`CollisionRaycastPoint` from 100 units above), copies the position into the model's root
   matrix translation, appends it to `g_stageObjList`, runs `ActorCrystalStandCheckOrigin` on it
   and returns it. A failed allocation is not checked (the copy goes through a NULL stand). */

CoreObject *ActorCrystalStandSpawnAtPoint(int index)
{
    float pos[4];
    bool fromLow;
    void *mem;
    ActorCrystalStand *stand = NULL;
    float *root;

    ActorStageObjRecordGetType4Pos(pos, index);
    pos[1] = pos[1] + 100.0f;
    CollisionRaycastPoint(pos, pos);
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(ActorCrystalStand), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
        ActorCrystalStandCtor(mem, "fz_crystal01_stand_60.gmo");
        stand = (ActorCrystalStand *)mem;
    }
    stand->base.pos[0] = pos[0];
    stand->base.pos[1] = pos[1];
    stand->base.pos[2] = pos[2];
    stand->base.pos[3] = pos[3];
    root = &stand->base.data->rootMatrix[12];
    root[0] = stand->base.pos[0];
    root[1] = stand->base.pos[1];
    root[2] = stand->base.pos[2];
    root[3] = stand->base.pos[3];
    CoreObjectListAppend((CoreObject *)stand, g_stageObjList);
    ActorCrystalStandCheckOrigin(stand);
    return (CoreObject *)stand;
}
