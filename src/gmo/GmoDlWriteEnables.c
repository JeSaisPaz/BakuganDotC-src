// bdc 0x089e10e0 GmoDlWriteEnables
#include "bdc.h"

/* GE enable-state filter of the GMO renderer: starts from the model defaults (`flags2e`), lets the
   material layer (`layerFlags14` selects, `layerFlags16` values) and then the mesh (`enableMask`
   selects, `enableBits` values) override the bits not locked by the model's `enableOverride`,
   stores the result in `lastEnables` and writes only the commands whose bits changed (all of
   them when `lastEnables` was -1) and are in the model's `flags30` mask: bit 0 lighting `0x17`,
   1 fog `0x1f`, 2 texture `0x1e`, 3 `0x1d`, 4 `0x23`, 5 `0xe7` (inverted), 6 `0x22`, 7 `0xe9`
   (0xff when clear), 8 `0x9b`, 9 `0x51` plus inverted `0x38`, 10-11 `0xc9`, 12-13 `0xc6`,
   14-15 `0xc7`. */

void GmoDlWriteEnables(GmoDlContext *self)
{
    GmoModel *model;
    u32 free;
    u32 materialSel;
    u32 meshSel;
    u32 enables;
    u32 last;
    u32 changed;
    u32 value;
    u32 extra;
    u32 *p;

    model = self->model;
    free = ~(u32)model->enableOverride;
    materialSel = self->material->layerFlags14 & free;
    meshSel = self->mesh->enableMask & free;
    enables = (((model->flags2e & ~materialSel) | (self->material->layerFlags16 & materialSel)) &
               ~meshSel) |
              (self->mesh->enableBits & meshSel);
    last = self->lastEnables;
    self->lastEnables = enables;
    changed = 0xffffffff;
    if (last != 0xffffffff) {
        changed = enables ^ last;
    }
    changed &= model->flags30;
    if (changed == 0) {
        return;
    }
    if ((changed & 0x1) != 0) {
        p = self->cur;
        self->cur = p + 1;
        *p = ((enables & 0x1) != 0) | 0x17000000;
    }
    if ((changed & 0x2) != 0) {
        p = self->cur;
        self->cur = p + 1;
        *p = ((enables & 0x2) != 0) | 0x1f000000;
    }
    if ((changed & 0x4) != 0) {
        p = self->cur;
        self->cur = p + 1;
        *p = ((enables & 0x4) != 0) | 0x1e000000;
    }
    if ((changed & 0x8) != 0) {
        p = self->cur;
        self->cur = p + 1;
        *p = ((enables & 0x8) != 0) | 0x1d000000;
    }
    if ((changed & 0x10) != 0) {
        p = self->cur;
        self->cur = p + 1;
        *p = ((enables & 0x10) != 0) | 0x23000000;
    }
    if ((changed & 0x20) != 0) {
        p = self->cur;
        self->cur = p + 1;
        *p = ((enables & 0x20) == 0) | 0xe7000000;
    }
    if ((changed & 0x40) != 0) {
        p = self->cur;
        self->cur = p + 1;
        *p = ((enables & 0x40) != 0) | 0x22000000;
    }
    if ((changed & 0x80) != 0) {
        value = 0xff;
        if ((enables & 0x80) != 0) {
            value = 0;
        }
        p = self->cur;
        self->cur = p + 1;
        *p = value | 0xe9000000;
    }
    if ((changed & 0x100) != 0) {
        p = self->cur;
        self->cur = p + 1;
        *p = ((enables & 0x100) != 0) | 0x9b000000;
    }
    if ((changed & 0x200) != 0) {
        p = self->cur;
        self->cur = p + 1;
        *p = ((enables & 0x200) != 0) | 0x51000000;
        p = self->cur;
        self->cur = p + 1;
        *p = ((enables & 0x200) == 0) | 0x38000000;
    }
    if ((changed & 0xc00) != 0) {
        p = self->cur;
        self->cur = p + 1;
        *p = ((u32)((enables & 0x800) != 0) << 8) | 0xc9000000 | ((enables & 0x400) == 0);
    }
    if ((changed & 0x3000) != 0) {
        value = 4;
        extra = 0;
        if ((enables & 0x1000) != 0) {
            extra = 1;
        }
        if ((enables & 0x2000) != 0) {
            value = 6;
        }
        p = self->cur;
        self->cur = p + 1;
        *p = (extra << 8) | 0xc6000000 | extra | value;
    }
    if ((changed & 0xc000) != 0) {
        p = self->cur;
        self->cur = p + 1;
        *p = ((u32)((enables & 0x8000) == 0) << 8) | 0xc7000000 | ((enables & 0x4000) == 0);
    }
}
