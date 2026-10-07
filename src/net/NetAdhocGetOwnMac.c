// bdc 0x089d3d54 NetAdhocGetOwnMac
#include "bdc.h"

/* Returns a pointer to the local WLAN MAC address stored in the ad-hoc manager
   (`g_netAdhoc``+0x14`, filled by `NetAdhocLoadModules` with `sceWlanGetEtherAddr`), or NULL
   when networking is off. */

u8 *NetAdhocGetOwnMac(void)

{
  u8 *mac;
  
  mac = (u8 *)0x0;
  if (g_netAdhoc != (NetAdhocManager *)0x0) {
    mac = g_netAdhoc->ownMac;
  }
  return mac;
}

