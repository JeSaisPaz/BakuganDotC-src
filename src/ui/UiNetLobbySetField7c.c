// bdc 0x08941a40 UiNetLobbySetField7c
#include "bdc.h"

/* Stores `value` in `field7c` (`+0x7c`) of `UiNetLobby`. */

void UiNetLobbySetField7c(UiNetLobby *self, s32 value)

{
  self->field7c = value;
  return;
}

