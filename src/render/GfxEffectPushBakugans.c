// bdc 0x0881d970 GfxEffectPushBakugans
#include "bdc.h"

/* Effect command that pushes nearby Bakugan away from the `effect`: for every unit
   of `BtlGetBakuganList` with `playerSlot < 5` whose kind (virtual slot 20) is not the excluded
   kind `g_btlMapFlashKind`, if its vertical distance to the effect position is below `height`
   and its horizontal distance below `radius`, adds to its `tiltPush` a horizontal impulse along the
   Bakugan→effect offset direction (strength `sqrt(radius² − d²)·0.02`, softened in the outer
   quarter, at most 25) rotated by `angle` about Y. Nothing happens outside a battle.
   The original read the bank constants S703 (2/π, angle → vrot quarter turns) and S713 (0, the
   scale for a zero-length offset); here they are the plain angle and 0.0f. */

void GfxEffectPushBakugans(float radius, float angle, float height, GfxEffect *effect)
{
    BtlBakugan **list;
    BtlBakugan *unit;
    const VtblEntry *vtbl;
    s32 kind;
    float radiusSq;
    float inner;
    float outer;
    float push;
    float dx;
    float dy;
    float dz;
    float distSq;
    float lenSq;
    float scale;
    float nx;
    float ny;
    float nz;
    float c;
    float s;

    if (BtlGetBakuganList() == NULL) {
        return;
    }
    list = BtlGetBakuganList();
    radiusSq = radius * radius;
    inner = radius * 0.75f;
    outer = radius * 0.25f;
    for (unit = *list; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
        if (unit->playerSlot >= 5) {
            continue;
        }
        vtbl = (const VtblEntry *)unit->base.base.vtable;
        kind = ((s32 (*)(void *))vtbl[20].fn)((u8 *)unit + vtbl[20].delta);
        if (kind == g_btlMapFlashKind) {
            continue;
        }
        dx = effect->pos[0] - unit->base.pos[0];
        dy = effect->pos[1] - unit->base.pos[1];
        dz = effect->pos[2] - unit->base.pos[2];
        if (!(__builtin_fabsf(dy) < height)) {
            continue;
        }
        dy = 0.0f;
        distSq = dx * dx + dy * dy + dz * dz;
        if (radiusSq <= distSq) {
            continue;
        }
        push = __builtin_sqrtf(radiusSq - distSq);
        if (!(push <= inner)) {
            push = push - ((push - inner) / outer) * radius;
        }
        push = push * 0.02f;
        if (!(push <= 25.0f)) {
            push = 25.0f;
        }
        /* normalize (dx, 0, dz) and scale by push; a zero offset gives a zero vector */
        lenSq = dx * dx + dy * dy + dz * dz;
        if (lenSq == 0.0f) {
            scale = 0.0f;
        } else {
            scale = VfRsq(lenSq);
        }
        scale = scale * push;
        nx = dx * scale;
        ny = dy * scale;
        nz = dz * scale;
        /* rotate by angle about Y: (x·cos − z·sin, y, x·sin + z·cos) */
        c = __builtin_cosf(angle);
        s = __builtin_sinf(angle);
        unit->tiltPush[0] = unit->tiltPush[0] + (nx * c - nz * s);
        unit->tiltPush[1] = unit->tiltPush[1] + ny;
        unit->tiltPush[2] = unit->tiltPush[2] + (nx * s + nz * c);
    }
}
