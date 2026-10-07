// bdc 0x08904c10 BtlDemoScbObjectCtor
#include "bdc.h"

/* Constructor of a `.scb` scene object entry (`BtlDemoScbObject`, 0x2c bytes): initialises the
   `CoreObject` base with no chain, installs `g_btlDemoScbObjectVtbl`, copies the 16-byte
   record `rec`, mirrors the record's first word into the base `unk08`, and stores the kind
   (`BtlDemoScbObjectKind` of the record id). Returns `obj`. */
CoreObject *BtlDemoScbObjectCtor(CoreObject *obj, void *rec)
{
    BtlDemoScbObject *self = (BtlDemoScbObject *)obj;

    CoreObjectInit(obj, NULL);
    obj->vtable = &g_btlDemoScbObjectVtbl;
    memcpy(&self->rec, rec, sizeof(BtlDemoScbRecord));
    obj->unk08 = self->rec.key;
    self->kind = BtlDemoScbObjectKind(obj, self->rec.id);
    return obj;
}
