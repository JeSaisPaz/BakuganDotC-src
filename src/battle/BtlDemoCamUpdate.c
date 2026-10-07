// bdc 0x088fddc0 BtlDemoCamUpdate
#include "bdc.h"

/* Update (vtable slot `+0x14`) of the battle demo camera (`BtlDemoCamCtor`): calls the method
   for `state` through the pointer-to-member table `g_btlDemoCamStateTable` (state 0 =
   `BtlDemoCamStateMain`), rebuilds the view (`GfxCameraUpdate` flags 1), then sets the
   sound-listener point to the eye and moves it halfway toward the followed object's position:
   `listener = eye + (target.pos - eye) * 0.5` on all four lanes. */

void BtlDemoCamUpdate(BtlDemoCam *self)
{
    const MemberFnPtr *e;
    u8 *obj;
    void *fn;

    e = &g_btlDemoCamStateTable[self->state];
    obj = (u8 *)self + e->delta;
    fn = e->pfn;
    if (e->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)e->pfn);
        const VtblEntry *entry = &vtbl[e->index];

        obj += entry->delta;
        fn = entry->fn;
    }
    ((void (*)(void *))fn)(obj);
    GfxCameraUpdate(&self->base, 1);
    /* 16-byte quad copy of the eye */
    self->listener[0] = self->base.eye[0];
    self->listener[1] = self->base.eye[1];
    self->listener[2] = self->base.eye[2];
    self->listener[3] = self->base.eye[3];
    /* listener += (target.pos - listener) * 0.5 */
    {
        const float *tpos = ((GfxModel *)self->target)->pos;
        float l0 = self->listener[0], l1 = self->listener[1];
        float l2 = self->listener[2], l3 = self->listener[3];

        self->listener[0] = l0 + (tpos[0] - l0) * 0.5f;
        self->listener[1] = l1 + (tpos[1] - l1) * 0.5f;
        self->listener[2] = l2 + (tpos[2] - l2) * 0.5f;
        self->listener[3] = l3 + (tpos[3] - l3) * 0.5f;
    }
}
