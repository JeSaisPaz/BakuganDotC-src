// bdc 0x088d6344 GameGimmickCollectionBoxUpdateLid
#include "bdc.h"

/* Lid logic of the collection box gimmick (`GameGimmickCollectionBoxCtor`, vtables
   `0x08af2f8c`/`0x08af302c`), only while a player exists (`ActorFindPlayer`): when the player
   is within 16.5 units of `triggerPos` (`GameIsPlayerWithinRange`) and in front of the box
   (`GameGetPlayerFacingSectorFrom` = 1) and `lidState` is not 0, plays the open motion
   (`motionName[0]`) plus virtual slot 6 (1.0f) and sound `0x2c00042`, sets `lidState = 0`, and
   sets `contactFlags` bit 1 (also when already open); when out of range and `lidState` is not 1,
   plays the close motion (`motionName[1]`), sets `lidState = 1`, and clears bit 1. In range but
   not in front: nothing. */

void GameGimmickCollectionBoxUpdateLid(GameGimmickCollectionBox *obj)
{
    ScePspFVector4 trigger;
    float pos[4];
    const VtblEntry *e;

    if (ActorFindPlayer() == NULL) {
        return;
    }
    trigger = obj->triggerPos;
    if (GameIsPlayerWithinRange(16.5f, obj, &trigger.x)) {
        pos[0] = obj->base.base.pos[0];
        pos[1] = obj->base.base.pos[1];
        pos[2] = obj->base.base.pos[2];
        pos[3] = obj->base.base.pos[3];
        if (GameGetPlayerFacingSectorFrom(obj, pos) != 1) {
            return;
        }
        if (obj->lidState != 0) {
            GfxModelEnableMotion(&obj->base.base);
            GfxModelPlayMotionByName(0.2f, &obj->base.base, obj->motionName[0], false);
            e = (const VtblEntry *)obj->base.base.base.vtable + 6;
            ((void (*)(void *, float))e->fn)((char *)obj + e->delta, 1.0f);
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x2c00042, 0, 0);
            }
            obj->lidState = 0;
        }
        obj->base.contactFlags |= 2;
    } else {
        if (obj->lidState != 1) {
            GfxModelEnableMotion(&obj->base.base);
            GfxModelPlayMotionByName(0.2f, &obj->base.base, obj->motionName[1], false);
            e = (const VtblEntry *)obj->base.base.base.vtable + 6;
            ((void (*)(void *, float))e->fn)((char *)obj + e->delta, 1.0f);
            obj->lidState = 1;
        }
        obj->base.contactFlags &= ~2;
    }
}
