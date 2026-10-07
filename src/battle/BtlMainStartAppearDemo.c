// bdc 0x0884cbe4 BtlMainStartAppearDemo
#include "bdc.h"

/* Creates the Bakugan appear demo (task 0x67, BtlAppearDemoCreate) for the active
   team Bakugan (SaveGetTeamBakuganOrMode1(-1)), hands it the battle's model lists
   1 and 2 as its map-model and model lists, and clears g_btlCameraDefaultMode. */
void BtlMainStartAppearDemo(BtlMain *self)
{
    BtlDemo *demo;

    demo = (BtlDemo *)BtlAppearDemoCreate(SaveGetTeamBakuganOrMode1(-1));
    demo->mapModels = &self->modelLists[1];
    demo->models = &self->modelLists[2];
    g_btlCameraDefaultMode = 0;
}
