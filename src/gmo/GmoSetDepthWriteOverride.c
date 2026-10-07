// bdc 0x089e1044 GmoSetDepthWriteOverride
#include "bdc.h"

/* Sets the byte `0x08ac5bec` read by `GmoDlWriteMeshRenderState`: while it is set, materials
   whose depth-bias byte (`state+6`) is 1 (the `__DE` material-name code, depth offset +90) are
   drawn with depth writes disabled (`ZMSK` `0xe7000001`). */

void GmoSetDepthWriteOverride(bool on)

{
  g_gmoDepthWriteOverride = on;
  return;
}

