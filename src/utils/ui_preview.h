#ifndef UI_PREVIEW_H
#define UI_PREVIEW_H

/* Set to 1 to preview VLAN UI with fixed mock data (no real VLAN source). */
#ifndef UI_VLAN_PREVIEW
#define UI_VLAN_PREVIEW 0
#endif

/* When UI_VLAN_PREVIEW=1: simulate 1 or 2 VLAN pages. */
#ifndef UI_VLAN_PREVIEW_COUNT
#define UI_VLAN_PREVIEW_COUNT 2
#endif

#endif
