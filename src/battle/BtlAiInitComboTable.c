// bdc 0x08897dd8 BtlAiInitComboTable
#include "bdc.h"

/* Builds the combo-input table `comboTable` of `BtlAi` for its owner unit
   (`BtlAiComboTableInit``(&self->comboTable, self->owner)`). Called by `BtlAiCreate`. */
void BtlAiInitComboTable(BtlAi *self)
{
    BtlAiComboTableInit(&self->comboTable, self->owner);
}
