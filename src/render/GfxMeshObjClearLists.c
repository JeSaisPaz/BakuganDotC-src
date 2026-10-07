// bdc 0x08825540 GfxMeshObjClearLists
#include "bdc.h"

/* Resets the six mesh-object lists `0x08b00200`, `0x08b00210`, `0x08b00220`, `0x08b00230`,
   `0x08b00240`, `0x08b00250` (0x10-byte `CoreObject` list heads) to empty (all words zero).
   Called when a battle/field/demo loads (`GameFieldPhaseLoad`, `BtlDemoStateLoad`, …). */

void GfxMeshObjClearLists(void)
{
  g_gfxMeshObjList0.tail = NULL;
  g_gfxMeshObjList0.head = NULL;
  g_gfxMeshObjList0.count = 0;
  g_gfxMeshObjList1.tail = NULL;
  g_gfxMeshObjList1.head = NULL;
  g_gfxMeshObjList1.count = 0;
  g_gfxMeshObjList2.tail = NULL;
  g_gfxMeshObjList2.head = NULL;
  g_gfxMeshObjList2.count = 0;
  g_gfxMeshObjList3.tail = NULL;
  g_gfxMeshObjList3.head = NULL;
  g_gfxMeshObjList3.count = 0;
  g_gfxMeshObjList4.tail = NULL;
  g_gfxMeshObjList4.head = NULL;
  g_gfxMeshObjList4.count = 0;
  g_gfxMeshObjList5.tail = NULL;
  g_gfxMeshObjList5.head = NULL;
  g_gfxMeshObjList5.count = 0;
}
