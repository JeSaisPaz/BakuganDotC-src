// bdc 0x08825964 GfxMeshObjUpdateAll
#include "bdc.h"

/* Runs the state handler (`GfxMeshObjRunState`) of every mesh object (`GfxMeshObjCtor`) in the
   six mesh-object lists `0x08b00200`, `0x08b00210`, `0x08b00220`, `0x08b00230`, `0x08b00240`,
   `0x08b00250` (0x10-byte `CoreObject` list heads). Called once per frame by battle/field updates
   (`BtlFinishTaskUpdate`, `BtlMainUpdateScene`, …). */

void GfxMeshObjUpdateAll(void)
{
  GfxMeshObj *obj;
  GfxMeshObj *next;

  for (obj = (GfxMeshObj *)g_gfxMeshObjList0.head; obj != NULL; obj = next) {
    next = (GfxMeshObj *)obj->base.next;
    GfxMeshObjRunState(obj);
  }
  for (obj = (GfxMeshObj *)g_gfxMeshObjList1.head; obj != NULL; obj = next) {
    next = (GfxMeshObj *)obj->base.next;
    GfxMeshObjRunState(obj);
  }
  for (obj = (GfxMeshObj *)g_gfxMeshObjList2.head; obj != NULL; obj = next) {
    next = (GfxMeshObj *)obj->base.next;
    GfxMeshObjRunState(obj);
  }
  for (obj = (GfxMeshObj *)g_gfxMeshObjList3.head; obj != NULL; obj = next) {
    next = (GfxMeshObj *)obj->base.next;
    GfxMeshObjRunState(obj);
  }
  for (obj = (GfxMeshObj *)g_gfxMeshObjList4.head; obj != NULL; obj = next) {
    next = (GfxMeshObj *)obj->base.next;
    GfxMeshObjRunState(obj);
  }
  for (obj = (GfxMeshObj *)g_gfxMeshObjList5.head; obj != NULL; obj = next) {
    next = (GfxMeshObj *)obj->base.next;
    GfxMeshObjRunState(obj);
  }
}
