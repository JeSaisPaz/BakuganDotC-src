// bdc 0x08941760 UiNetLobbySetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of the ad-hoc lobby screen (task 2000): indices 0-2 go to
   `CoreTaskSetField`; 3 sets the phase (resetting `phaseStep` on change); 4 sets `phaseStep`
   directly; 5 selects the lobby mode: value 0 → phase 1 (host), 1 → phase 2 (join), resetting
   `phaseStep` on change. */

void UiNetLobbySetField(UiNetLobby *self, u32 index, u32 value)

{
  int cur;
  int next;

  if (index < 3) {
    CoreTaskSetField((CoreTask *)self,index,value);
    return;
  }
  if ((int)index < 4) {
    if ((2 < (int)index) && ((self->base).phase != value)) {
      (self->base).phase = value;
      (self->base).phaseStep = 0;
      return;
    }
  }
  else {
    if ((int)index < 5) {
      (self->base).phaseStep = value;
      return;
    }
    if ((int)index < 6) {
      cur = (self->base).phase;
      next = cur;
      if ((int)value < 1) {
        if (-1 < (int)value) {
          next = 1;
        }
      }
      else if ((int)value < 2) {
        next = 2;
      }
      if (next != cur) {
        (self->base).phase = next;
        (self->base).phaseStep = 0;
      }
    }
  }
  return;
}
