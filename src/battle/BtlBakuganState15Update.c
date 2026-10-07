// bdc 0x088757c4 BtlBakuganState15Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 15 (`state`), vtable slot `+0x148` called through
   `BtlBakuganRunState`: the unit's fade-out/vanish sequence. For object kinds (`unk08`) 0x14,
   0x12 and 8 it first asks the battle intro demo (task 0x65, `BtlDemo`) whether it has reached
   its release point (demo 100 or 0x34, demo 0x5e from scene frame 150, demo 0x36 from frame 75);
   while it has not and no demo skip is requested (`g_btlDemoSkipRequest`) it sets state flag
   0x8000000. `subTimer` drives the sequence: 0xdead starts it (focus point {0,0,0,0.5} through
   `BtlDemoSetFocusPointIfActive`, `subWait` = 35, next step), 0xdeae runs it, any other value
   returns at once. Each running frame: t = clamp((35 - subWait) * 0.03, 0, 1); the attribute
   colour (`BtlBakuganGetAttributeColor`) with alpha (1 - cos(pi t)) / 2 goes to virtual entry 4;
   the "Bip01_Pelvis" bone scale xyz is multiplied by 1 - 0.15 (1 - cos(pi t)); from `subWait` < 16
   the ambient alpha drops by 1/16 per frame. When `subWait` runs out (post-decrement <= 0) fog is
   switched off, `subTimer` advances, the pelvis scale is reset to {1,1,1,0}, ambient alpha to 0
   and `g_btlDepthTestEnabled` is set. Finally `effectAnchor` = camera eye + 280 x camera dir
   (xyz), then eased 0.4 of the way toward `anchorMatrix[3]` (all four lanes).
   The clamp bounds (1, 0), the quarter-turn factor 2/pi and the pelvis scale w / camera offset w
   (0) were VFPU bank constants (S733, S713, S703) and are literals here. */

void BtlBakuganState15Update(BtlBakugan *self)
{
    float focus[4];
    float tint[4];
    const VtblEntry *entry;
    BtlDemo *demo;
    GmoNode *node;
    float *color;
    float t;
    float angle;
    float c;
    float scale;
    float k;
    s32 i;
    u32 kind;
    s32 timer;
    s32 wait;
    int released;

    kind = self->base.base.unk08;
    if (kind == 0x14 || kind == 0x12 || kind == 8) {
        released = 0;
        if (CoreTaskExists(0x65) != 0) {
            demo = (BtlDemo *)CoreTaskFind(0x65);
            if (demo != NULL) {
                switch (demo->demoId) {
                case 100:
                case 0x34:
                    released = 1;
                    break;
                case 0x5e:
                    if (BtlDemoGetFirstPlayerFrame(demo) >= 0x96) {
                        released = 1;
                    }
                    break;
                case 0x36:
                    if (BtlDemoGetFirstPlayerFrame(demo) >= 0x4b) {
                        released = 1;
                    }
                    break;
                }
            }
        }
        if (released == 0 && g_btlDemoSkipRequest == 0) {
            self->stateFlags |= 0x8000000;
        }
    }
    timer = self->subTimer;
    if (timer > 0xdead) {
        if (timer > 0xdeae) {
            return;
        }
    } else {
        if (timer <= 0xdeac) {
            return;
        }
        focus[0] = 0.0f;
        focus[1] = 0.0f;
        focus[2] = 0.0f;
        focus[3] = 0.5f;
        BtlDemoSetFocusPointIfActive(0x23, focus);
        self->subWait = 0x23;
        self->subTimer = self->subTimer + 1;
    }
    color = BtlBakuganGetAttributeColor(self);
    tint[0] = color[0];
    tint[1] = color[1];
    tint[2] = color[2];
    tint[3] = color[3];
    t = (float)(0x23 - self->subWait) * 0.0299999993f;
    /* vmin.s with 1, then vmax.s with 0 (t is finite) */
    t = t < 1.0f ? t : 1.0f;
    t = t > 0.0f ? t : 0.0f;
    angle = t * 3.14159274f;
    c = __builtin_cosf(angle);
    tint[3] = (1.0f - c) * 0.5f;
    entry = &((const VtblEntry *)self->base.base.vtable)[4];
    ((void (*)(void *, float *))entry->fn)((u8 *)self + entry->delta, tint);
    c = __builtin_cosf(angle);
    scale = 1.0f - (1.0f - c) * 0.5f * 0.300000012f;
    node = (GmoNode *)GfxModelFindNode(&self->base, "Bip01_Pelvis");
    node->scale[0] = node->scale[0] * scale;
    node->scale[1] = node->scale[1] * scale;
    node->scale[2] = node->scale[2] * scale;
    node->scaleW = 0.0f;
    if (self->subWait < 0x10) {
        self->base.ambient[3] = self->base.ambient[3] - 0.0625f;
    }
    wait = self->subWait;
    self->subWait = wait - 1;
    if (wait <= 0) {
        self->base.fogEnabled = 0;
        self->subTimer = self->subTimer + 1;
        node = (GmoNode *)GfxModelFindNode(&self->base, "Bip01_Pelvis");
        node->scale[0] = 1.0f;
        node->scale[1] = 1.0f;
        node->scale[2] = 1.0f;
        node->scaleW = 0.0f;
        self->base.ambient[3] = 0.0f;
        g_btlDepthTestEnabled = 1;
    }
    /* effectAnchor = eye; effectAnchor.xyz += dir.xyz * 280;
       effectAnchor += (anchorMatrix[3] - effectAnchor) * 0.4 (all four lanes) */
    for (i = 0; i < 4; i++) {
        self->effectAnchor[i] = g_gfxActiveCamera->eye[i];
    }
    for (i = 0; i < 3; i++) {
        k = g_gfxActiveCamera->dir[i] * 280.0f;
        self->effectAnchor[i] = self->effectAnchor[i] + k;
    }
    for (i = 0; i < 4; i++) {
        k = (self->anchorMatrix[3][i] - self->effectAnchor[i]) * 0.400000006f;
        self->effectAnchor[i] = self->effectAnchor[i] + k;
    }
}
