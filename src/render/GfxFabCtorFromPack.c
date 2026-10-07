// bdc 0x089f83dc GfxFabCtorFromPack
#include "bdc.h"

/* Constructor of a `.fab` 2D animation object (vtable `0x08af5874`) whose file is looked up by name
   in the loaded packs: base node ctor `CoreObjectInitInList(fab, parent)`, vtable, then
   `GfxFabLoadFromPack``(fab, name)`. Returns `fab`. */

GfxFab *GfxFabCtorFromPack(GfxFab *fab, char *name, void *parent)
{
    CoreObjectInitInList((CoreObject *)fab, parent);
    fab->vtable = &g_gfxFabVtable;
    GfxFabLoadFromPack(fab, name);
    return fab;
}
