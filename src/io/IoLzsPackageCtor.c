// bdc 0x089bd974 IoLzsPackageCtor
#include "bdc.h"

/* Constructor of an `IoLzsPackage` (0x44 bytes, embedded in tasks, on the stack or in the
   `ScriptOpPackage` array): runs `CoreNodeCtor` with no anchor, installs vtable
   `g_ioLzsPackageVtbl` and clears the load request `+0x2c`, directory `+0x24`, registered flag
   `+0x2a`, owner `+0x30`, textures `+0x38` and texture count `+0x3c`. Returns `self`. */

IoLzsPackage *IoLzsPackageCtor(IoLzsPackage *self)
{
  CoreNodeCtor(&self->base, NULL);
  self->base.vtable = g_ioLzsPackageVtbl;
  self->request = NULL;
  self->dir = NULL;
  self->registered = 0;
  self->owner = NULL;
  self->textures = NULL;
  self->textureCount = 0;
  return self;
}
