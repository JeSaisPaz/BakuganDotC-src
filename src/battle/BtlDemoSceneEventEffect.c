// bdc 0x08905db8 BtlDemoSceneEventEffect
#include "bdc.h"

/* Effect event handler of the battle demo scene player task (task id 0x66,
   `BtlDemoScenePlayerCtor`): at the event's frame (`ev+0x18` == `player+0x30`) and with no delay
   (`+0x1a` == 0) it spawns the effect whose id is the event's first parameter
   (`BtlDemoSceneSpawnEffect`) at a position chosen by the event kind `+0x2c`: 0 = the parameters
   2–4 (`ev+0x28`, count `+0x30`; y = 1 when all three are ±0), 1 = the unit's `"Bip01"` node with
   y = 0 (`GfxModelGetNodeWorldPos` on `player+0x18`; the origin without a unit), 2 = the origin,
   0xe = the origin with a per-demo effect id from `g_btlDemoEffectIdTable`; kinds 3..0xd do
   nothing. Every position starts as the bank zero vector C720 (w = 0). With a delay below 2 it
   instead clears the demo's flash flag (`demo+0x758`, alpha `+0x684 = 0.3`). Then deletes the event
   (virtual destructor, flags 3). */

/* Parameter `i` of the event, or NULL past its parameter count. */
static BtlDemoScbEffectParam *EffectParamAt(const BtlDemoScbEffectEvent *event, s32 i)
{
    return event->paramCount > i ? &event->params[i] : NULL;
}

/* Value of a parameter as a float, 0 when absent. */
static float EffectParamFloat(const BtlDemoScbEffectParam *param)
{
    if (param == NULL) {
        return 0.0f;
    }
    return param->isFloat != 0 ? param->value.f : (float)param->value.i;
}

void BtlDemoSceneEventEffect(void *player, void *ev)
{
    BtlDemoScenePlayer *self = (BtlDemoScenePlayer *)player;
    BtlDemoScbEffectEvent *event = (BtlDemoScbEffectEvent *)ev;
    BtlDemoScbEffectParam *idParam;
    BtlDemo *demo;
    union {
        float f;
        u32 u;
    } bx, by, bz;
    ScePspFVector4 pos;
    ScePspFVector4 node;
    s32 id;

    if (event->base.frame != self->frame) {
        return;
    }
    if (event->base.flags != 0) {
        if (event->base.flags < 2) {
            demo = (BtlDemo *)self->demo;
            demo->flashOn = 0;
            demo->flashAlpha = 0.300000012f;
        }
    } else {
        /* C720 is the bank zero vector */
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        pos.w = 0.0f;
        switch (event->posKind) {
        case 0:
            idParam = EffectParamAt(event, 0);
            pos.x = EffectParamFloat(EffectParamAt(event, 1));
            pos.y = EffectParamFloat(EffectParamAt(event, 2));
            pos.z = EffectParamFloat(EffectParamAt(event, 3));
            bx.f = pos.x;
            by.f = pos.y;
            bz.f = pos.z;
            if (((bx.u | by.u | bz.u) & 0x7fffffff) == 0) {
                pos.y = 1.0f;
            }
            /* (the listing also adds pos.xyz into a stack identity matrix it never uses) */
            /* both tags read the id as the word at +4 */
            id = idParam != NULL ? idParam->value.i : 0;
            BtlDemoSceneSpawnEffect(self, id, &pos.x, NULL);
            break;
        case 1:
            idParam = EffectParamAt(event, 0);
            id = idParam != NULL ? idParam->value.i : 0;
            if (self->unit != NULL) {
                GfxModelGetNodeWorldPos((GfxModel *)self->unit, &node, "Bip01");
                pos = node;
                pos.y = 0.0f;
            }
            BtlDemoSceneSpawnEffect(self, id, &pos.x, NULL);
            break;
        case 2:
            idParam = EffectParamAt(event, 0);
            id = idParam != NULL ? idParam->value.i : 0;
            BtlDemoSceneSpawnEffect(self, id, &pos.x, NULL);
            break;
        case 0xe:
            BtlDemoSceneSpawnEffect(self, g_btlDemoEffectIdTable[(self->demoId - 0x17) / 4 + 1], &pos.x,
                                    NULL);
            break;
        default:
            break;
        }
    }
    if (event != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)event->base.base.vtable)[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)event + dtor->delta, 3);
    }
}
