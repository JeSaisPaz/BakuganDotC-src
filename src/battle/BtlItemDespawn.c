// bdc 0x088b6708 BtlItemDespawn
#include "bdc.h"

/* Despawns a battle pickup item without deleting it: records its current age
   as pickedUp (nonzero marks it gone), stops its glow effect on the item effect
   manager when one is attached, and queues its model for deferred deletion,
   clearing both pointers. */
void BtlItemDespawn(void *itemPtr)
{
    BtlItem *item = (BtlItem *)itemPtr;

    item->pickedUp = item->age;
    if (item->effect != NULL) {
        GfxEffectStopAttached(g_btlItemEffectMgr, -1, item->pos);
    }
    item->effect = NULL;
    if (item->model != NULL) {
        CoreObjectDeferDelete((CoreObject *)item->model, 0);
    }
    item->model = NULL;
}
