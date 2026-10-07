// bdc 0x088b3480 StopWallCtor
#include "bdc.h"

/* Constructor of a stop wall (`StopWallCreate`, 0x90 bytes): `CoreObjectInit` with no chain,
   vtable `g_stopWallVtbl`, clears the collider, the shape, `cornerB`/`cornerA` (bank C720 = 0), the
   two effect pointers, `id`/`state`/`timer`, zeroes `centre` and `rot` and sets `rot[1]` to -pi/2,
   then appends the wall to the list read from `g_stopWallList` (`CoreObjectListAppend`). When
   that list was NULL it calls `StopWallEnsureList` but still passes the NULL read before the call.
   Returns `self`. */

StopWall *StopWallCtor(StopWall *self)

{
  CoreObjectList *list;
  int i;

  CoreObjectInit(&self->base, NULL);
  self->base.vtable = g_stopWallVtbl;
  self->collider = NULL;
  self->shape = NULL;
  for (i = 0; i < 4; i++) {
    self->cornerB[i] = 0.0f;
  }
  for (i = 0; i < 4; i++) {
    self->cornerA[i] = 0.0f;
  }
  for (i = 0; i < 2; i++) {
    self->effect[i] = NULL;
  }
  self->id = 0;
  self->state = 0;
  self->timer = 0;
  for (i = 0; i < 4; i++) {
    self->centre[i] = 0.0f;
  }
  for (i = 0; i < 4; i++) {
    self->rot[i] = 0.0f;
  }
  self->rot[1] = -1.5707964f;
  list = g_stopWallList;
  if (list == NULL) {
    StopWallEnsureList();
  }
  CoreObjectListAppend(&self->base, list);
  return self;
}
