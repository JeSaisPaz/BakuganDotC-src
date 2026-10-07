// bdc 0x08a3239c GmoMotionRefNameEquals
#include "bdc.h"

/* `GmoMotionRef` virtual method (vtable `0x08af5424` entry 5, name equals): case-insensitive
   comparison (`strcasecmp`) of `ref->name` with `name`; true when equal. */

bool GmoMotionRefNameEquals(GmoMotionRef *ref, const char *name)

{
  int cmp;
  
  cmp = strcasecmp(ref->name,name);
  return cmp == 0;
}

