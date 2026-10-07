// bdc 0x088b64fc BtlItemCtor
#include "bdc.h"

/* Constructor of a battle pickup item (`BtlItem`, a `CoreObject`): runs `CoreObjectInit` with no
   chain, installs `g_btlItemVtbl`, copies the 16-byte `pos` into `pos` (`+0x20`) and from there into
   `basePos` (`+0x30`), initialises it with `BtlItemInit``(self, type)`, appends it to
   `g_btlItemList` (`CoreObjectListAppend`), clears `picker` (`+0x40`) and returns `self`. */

BtlItem *BtlItemCtor(BtlItem *self, s32 type, float *pos)
{
    CoreObjectInit(&self->base, NULL);
    self->base.vtable = &g_btlItemVtbl;
    self->pos[0] = pos[0];
    self->pos[1] = pos[1];
    self->pos[2] = pos[2];
    self->pos[3] = pos[3];
    self->basePos[0] = self->pos[0];
    self->basePos[1] = self->pos[1];
    self->basePos[2] = self->pos[2];
    self->basePos[3] = self->pos[3];
    BtlItemInit(self, type);
    CoreObjectListAppend(&self->base, (CoreObjectList *)&g_btlItemList);
    self->picker = NULL;
    return self;
}
