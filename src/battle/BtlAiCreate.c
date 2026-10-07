// bdc 0x0888de18 BtlAiCreate
#include "bdc.h"

/* Creates the AI controller of a CPU unit: allocates 0xa30 bytes from the low end of the heap
   (`MemAlloc` under `MemLock`, previous placement policy restored) and runs the base
   constructor `BtlAiCtor`; then stores the owner `unit` in `owner` (`+0x1a0`), builds the combo
   table (`BtlAiInitComboTable`), runs the kind setup `BtlAiSetupKind` with the unit's kind
   (`CoreObject` `unk08`, 0 for a NULL unit) and `BtlAiSetOwnerId` with its `playerSlot`
   (`+0x150`), and returns the AI. The binary does not guard the steps after the allocation: a
   failed allocation writes through NULL. The unit constructor (`BtlCpuUnitCtor`) stores the
   result at `unit->+0x6cc`; its level is set through `BtlBakuganSetAiLevel` /
   `BtlAiSetLevel`. */
BtlAi *BtlAiCreate(BtlBakugan *unit)
{
    bool fromLow;
    BtlAi *self;
    s32 kind;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    self = MemAlloc(sizeof(BtlAi), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (self != NULL) {
        BtlAiCtor(self);
    }
    self->owner = unit;
    BtlAiInitComboTable(self);
    kind = 0;
    if (unit != NULL) {
        kind = (s32)unit->base.base.unk08;
    }
    BtlAiSetupKind(self, kind);
    BtlAiSetOwnerId(self, unit->playerSlot);
    return self;
}
