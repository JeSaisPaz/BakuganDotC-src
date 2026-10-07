// bdc 0x0885e348 BtlBakuganCreateAttachments
#include "bdc.h"

/* Constructor helper of `BtlBakuganCtor`: creates the kind-specific (`base.base.unk08`)
   attachment objects. Kinds 8/0x14: two effects 0x96 on `g_worldEffectMgr` in `legs[0..1]`
   (`GfxEffectCreate`, owned by this unit) and a 0x140-byte sword blur on the `"Bip01_yari"`
   bone (`BtlSwordBlurInit`) in `weapon`; kind 0x12: the blur on `"Bip01_sword"`; kind 0xe: the
   `"afx_129m.gmo"` wing model (`GfxModelCtor`) in `wingModel`, its node 0 scaled to
   (0.1, -0.1, 0.1) and rotated by the quaternion (sin(a), 0, 0, cos(a)), a = (pi/2 * 1/pi) quarter
   turns (pi/4), multiplied on the right, and ambient
   0.4 on stages 4..7 (`GameStageIs4To7`); kinds 0xd/10: 2/4 attached effects 0xe3/0xe7
   (`GfxEffectSpawnAttached`) whose `anchorPos` is set to (0, 0, 0, 0) (the VFPU bank column C720)
   and becomes the effect's attach point;
   kind 0x1a binds `"10_D_Hades_Refrec"` to texture slot 6 and runs
   `BtlBakuganHadesMaterialCallback`; kind 0x11 runs `BtlBakuganMaterialEnableDepthWrite`
   on every material. A failed allocation leaves `weapon`/`wingModel` NULL. */
void BtlBakuganCreateAttachments(BtlBakugan *self)
{
    float rot[4];
    float q[4];
    float a;
    bool fromLow;
    void *mem;
    void *obj;
    GmoNode *node;
    GfxEffect *effect;
    s32 i;

    if (self->base.base.unk08 == 8 || self->base.base.unk08 == 0x14) {
        self->legs[0] = GfxEffectCreate(g_worldEffectMgr, 0x96);
        self->legs[1] = GfxEffectCreate(g_worldEffectMgr, 0x96);
        effect = self->legs[0];
        effect->ownerBakugan = self;
        if (self != NULL) {
            effect->ownerId = self->base.base.id;
        }
        effect = self->legs[1];
        effect->ownerBakugan = self;
        if (self != NULL) {
            effect->ownerId = self->base.base.id;
        }
        obj = NULL;
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = MemAlloc(sizeof(BtlSwordBlur), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (mem != NULL) {
            BtlSwordBlurInit(mem, self, GfxModelFindNode(&self->base, "Bip01_yari"));
            obj = mem;
        }
        self->weapon = obj;
    } else if (self->base.base.unk08 == 0x12) {
        obj = NULL;
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = MemAlloc(sizeof(BtlSwordBlur), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (mem != NULL) {
            BtlSwordBlurInit(mem, self, GfxModelFindNode(&self->base, "Bip01_sword"));
            obj = mem;
        }
        self->weapon = obj;
    } else if (self->base.base.unk08 == 0xe) {
        obj = NULL;
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = MemAlloc(sizeof(GfxModel), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (mem != NULL) {
            GfxModelCtor(mem, "afx_129m.gmo", 0);
            obj = mem;
        }
        self->wingModel = obj;
        node = GfxModelGetNode(obj, 0);
        node->scale[0] = 0.100000001f;
        node->scale[1] = -0.100000001f;
        node->scale[2] = 0.100000001f;
        node->scaleW = 0.0f;
        /* vrot.q [S,0,0,C] of (pi/2) * (1/pi) quarter turns */
        a = 1.57079637f * 0.318309873f;
        rot[0] = VfSinQuarter(a);
        rot[1] = 0.0f;
        rot[2] = 0.0f;
        rot[3] = VfCosQuarter(a);
        node = GfxModelGetNode(self->wingModel, 0);
        /* vqmul.q: rotate (x) rot */
        q[0] = node->rotate[0] * rot[3] + node->rotate[1] * rot[2] - node->rotate[2] * rot[1] + node->rotate[3] * rot[0];
        q[1] = -node->rotate[0] * rot[2] + node->rotate[1] * rot[3] + node->rotate[2] * rot[0] + node->rotate[3] * rot[1];
        q[2] = node->rotate[0] * rot[1] - node->rotate[1] * rot[0] + node->rotate[2] * rot[3] + node->rotate[3] * rot[2];
        q[3] = -node->rotate[0] * rot[0] - node->rotate[1] * rot[1] - node->rotate[2] * rot[2] + node->rotate[3] * rot[3];
        node->rotate[0] = q[0];
        node->rotate[1] = q[1];
        node->rotate[2] = q[2];
        node->rotate[3] = q[3];
        if (GameStageIs4To7() != 0) {
            self->base.ambient[0] = 0.400000006f;
            self->base.ambient[1] = 0.400000006f;
            self->base.ambient[2] = 0.400000006f;
            self->base.ambient[3] = 1.0f;
        }
    } else if (self->base.base.unk08 == 0xd) {
        for (i = 0; i < 2; i++) {
            self->legs[i] = GfxEffectSpawnAttached(g_worldEffectMgr, 0xe3, self->base.pos);
            /* sv.q of the bank zero vector C720 */
            self->legs[i]->anchorPos[0] = 0.0f;
            self->legs[i]->anchorPos[1] = 0.0f;
            self->legs[i]->anchorPos[2] = 0.0f;
            self->legs[i]->anchorPos[3] = 0.0f;
            self->legs[i]->attachPos = self->legs[i]->anchorPos;
            GfxEffectUpdateNow(self->legs[i]);
        }
    } else if (self->base.base.unk08 == 10) {
        for (i = 0; i < 4; i++) {
            self->legs[i] = GfxEffectSpawnAttached(g_worldEffectMgr, 0xe7, self->base.pos);
            /* sv.q of the bank zero vector C720 */
            self->legs[i]->anchorPos[0] = 0.0f;
            self->legs[i]->anchorPos[1] = 0.0f;
            self->legs[i]->anchorPos[2] = 0.0f;
            self->legs[i]->anchorPos[3] = 0.0f;
            self->legs[i]->attachPos = self->legs[i]->anchorPos;
            GfxEffectUpdateNow(self->legs[i]);
        }
    } else if (self->base.base.unk08 == 0x1a) {
        GfxSetTextureSlotB(GfxFindTexture("10_D_Hades_Refrec"), 6);
        GfxModelForEachMaterial(&self->base, (void *)BtlBakuganHadesMaterialCallback, NULL);
    } else if (self->base.base.unk08 == 0x11) {
        GfxModelForEachMaterial(&self->base, (void *)BtlBakuganMaterialEnableDepthWrite, NULL);
    }
}
