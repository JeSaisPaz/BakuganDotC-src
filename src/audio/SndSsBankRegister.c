// bdc 0x08a1fe84 SndSsBankRegister
#include "bdc.h"

/* Registers a sound bank with the Sony sound layer: stores the header pointer `phd` in `record[0]`
   and the sample-data pointer `pbd` in `record[1]`, and enters `record` in the first free entry of
   the 0x80-entry bank table (`g_sndSsBankTable`). Returns the table index (the bank id used by
   `SndSsKeyOn`), or an error code: `0x80450001` when the layer is not initialised, `0x8045000a`
   for a NULL argument, `0x8045000f` when the table is full (it then also clears the voice records
   at `g_sndSsVoiceVolumes`). */

s32 SndSsBankRegister(void **record, void *phd, void *pbd)

{
  u32 i;
  s32 v;

  if (g_sndSsState == -1) {
    return -0x7fbaffff;
  }
  if (record == NULL || phd == NULL || pbd == NULL) {
    return -0x7fbafff6;
  }
  for (i = 0; i < 0x80; i++) {
    if (g_sndSsBankTable[i] == NULL) {
      record[1] = pbd;
      g_sndSsBankTable[i] = record;
      record[0] = phd;
      return i;
    }
  }
  for (v = 0; v < 0x20; v++) {
    g_sndSsVoiceVolumes[v][1] = 0;
    g_sndSsVoiceVolumes[v][0] = 0;
    g_sndSsVoiceVolumes[v][3] = 0;
    g_sndSsVoiceVolumes[v][2] = 0;
  }
  return -0x7fbafff1;
}
