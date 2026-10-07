// bdc 0x08906890 BtlDemoSceneMapSoundId
#include "bdc.h"

/* Maps a sound cue number of the `.scb` scene files to the engine's sound id for the battle demo
   scene player: cues 0..0x2df (and negative cues, unchecked) are looked up in
   g_btlDemoSceneSoundIds; cues 0x33c..0x34d and 0x7f6..0x802 are hard-coded in two `switch`es
   (0xa0000a..0x100000b and 0x430001e..0x430002a); every other cue returns -1. */

s32 BtlDemoSceneMapSoundId(void *player, s32 cue)
{
  (void)player;

  if (cue < 0x2e0) {
    return g_btlDemoSceneSoundIds[cue];
  }
  if (cue < 0x34e) {
    switch (cue) {
    case 0x33c: return 0xa0000a;
    case 0x33d: return 0xa0000b;
    case 0x33e: return 0xa0000c;
    case 0x33f: return 0xc0000a;
    case 0x340: return 0xc0000b;
    case 0x341: return 0xc0000c;
    case 0x342: return 0xd0000a;
    case 0x343: return 0xd0000b;
    case 0x344: return 0xd0000c;
    case 0x345: return 0xe00009;
    case 0x346: return 0xe0000a;
    case 0x347: return 0xe0000b;
    case 0x348: return 0xf00009;
    case 0x349: return 0xf0000a;
    case 0x34a: return 0xf0000b;
    case 0x34b: return 0x1000009;
    case 0x34c: return 0x100000a;
    case 0x34d: return 0x100000b;
    }
  } else {
    switch (cue) {
    case 0x7f6: return 0x430001e;
    case 0x7f7: return 0x430001f;
    case 0x7f8: return 0x4300020;
    case 0x7f9: return 0x4300021;
    case 0x7fa: return 0x4300022;
    case 0x7fb: return 0x4300023;
    case 0x7fc: return 0x4300024;
    case 0x7fd: return 0x4300025;
    case 0x7fe: return 0x4300026;
    case 0x7ff: return 0x4300027;
    case 0x800: return 0x4300028;
    case 0x801: return 0x4300029;
    case 0x802: return 0x430002a;
    }
  }
  return -1;
}
