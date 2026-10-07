// bdc 0x088d7730 GameGimmickItemBoxStateBreak
#include "bdc.h"

/* Break state of the item box (breakable obstacle) gimmick (`GameGimmickItemBoxCtor`, vtables
   `0x08af30f4`/`0x08af319c`), by `step`: 0 plays sounds `0x2c00032`/`0x2c00039`, spawns effect
   0x3c 3 units above the box on `g_worldEffectMgr` (`GfxEffectSpawn`), lowers `breakAlpha`
   by 0.03 and goes to 1; 1 calls virtual slot 6 (1.0f) and goes to 2; 2 waits until
   `!(GfxModelMotionFrame < GfxModelGetMotionEnd)` and goes to 3; 3 clears the gimmick `state`
   and resets `step` to 0. Other steps: nothing. */

void GameGimmickItemBoxStateBreak(GameGimmickItemBox *obj)
{
    float pos[4];
    const VtblEntry *e;
    float frame;
    int step = obj->step;

    if (step < 2) {
        if (step < 0) {
            return;
        }
        if (step <= 0) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x2c00032, 0, 0);
            }
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x2c00039, 0, 0);
            }
            pos[0] = obj->base.base.pos[0];
            pos[1] = obj->base.base.pos[1];
            pos[2] = obj->base.base.pos[2];
            pos[3] = obj->base.base.pos[3];
            pos[1] = pos[1] + 3.0f;
            GfxEffectSpawn(g_worldEffectMgr, 0x3c, pos);
            obj->breakAlpha = obj->breakAlpha - 0.03f;
            obj->step = 1;
        } else {
            e = (const VtblEntry *)obj->base.base.base.vtable + 6;
            ((void (*)(void *, float))e->fn)((char *)obj + e->delta, 1.0f);
            obj->step = 2;
        }
    } else if (step < 3) {
        frame = GfxModelMotionFrame(&obj->base.base);
        if (!(frame < GfxModelGetMotionEnd(&obj->base.base))) {
            obj->step = 3;
        }
    } else if (step < 4) {
        obj->base.state = 0;
        obj->step = 0;
    }
}
