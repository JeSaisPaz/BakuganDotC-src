// bdc 0x08876cc0 BtlAttackCtor
#include "bdc.h"

/* Constructor of a battle attack object (0x160 bytes): `CoreObjectInit` with no parent,
   installs `g_btlAttackVtbl`, stores the owner unit and attack type, runs
   `BtlAttackInit``(self, type)` and appends the object to `g_btlAttackList`
   (`CoreObjectListAppend`). Returns `self`. */
BtlAttack *BtlAttackCtor(BtlAttack *self, void *owner, s32 type)
{
    CoreObjectInit(&self->base, NULL);
    self->base.vtable = g_btlAttackVtbl;
    self->owner = owner;
    self->type = type;
    BtlAttackInit(self, type);
    /* g_btlAttackList is typed as its head word; the holder (head/tail/count) starts there. */
    CoreObjectListAppend(&self->base, (CoreObjectList *)&g_btlAttackList);
    return self;
}
