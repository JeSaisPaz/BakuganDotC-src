// bdc 0x089f842c GfxFabCtor
#include "bdc.h"

/* Constructor of a `.fab` animation object from file data already in memory: base node ctor with
   `parent`, vtable `0x08af5874`, stores `name` at `+0x1c` and runs `GfxFabInit``(fab, data)`.
   Returns `fab`. */

GfxFab *GfxFabCtor(GfxFab *fab, char *name, void *data, void *parent)
{
  CoreObjectInitInList((CoreObject *)fab, parent);
  fab->vtable = &g_gfxFabVtable;
  fab->name = name;
  GfxFabInit(fab, data);
  return fab;
}
