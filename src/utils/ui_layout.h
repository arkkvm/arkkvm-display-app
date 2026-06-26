#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

/* Plain / VLAN shared field layout (absolute coords on main page) */
#define UI_PLAIN_X               26
#define UI_PLAIN_IPV6_Y          138
#define UI_PLAIN_IPV6_W          210
#define UI_PLAIN_IPV6_H          32
#define UI_PLAIN_Y_IP_NO_V6      103
#define UI_PLAIN_Y_IP_VAL_NO_V6  117
#define UI_PLAIN_Y_MAC_NO_V6     160
#define UI_PLAIN_Y_MAC_VAL_NO_V6 173
#define UI_PLAIN_Y_IP_HAS_V6     87
#define UI_PLAIN_Y_IP_VAL_HAS_V6 101
#define UI_PLAIN_Y_MAC_HAS_V6    175
#define UI_PLAIN_Y_MAC_VAL_HAS_V6 189

/* VLAN + IPv6: tag above IP; IPv4 lower, IPv6 tight under IPv4 value, MAC higher */
#define UI_VLAN_Y_IP_HAS_V6        92
#define UI_VLAN_Y_IP_VAL_HAS_V6    106
#define UI_VLAN_Y_IPV6_HAS_V6      136
#define UI_VLAN_Y_MAC_HAS_V6       168
#define UI_VLAN_Y_MAC_VAL_HAS_V6   181

/* VLAN-only: tag sits above the plain-style IP block */
#define UI_VLAN_TAG_X            UI_PLAIN_X
#define UI_VLAN_TAG_Y            70
#define UI_VLAN_TAG_H            16
#define UI_VLAN_TAG_RADIUS       4
#define UI_VLAN_TAG_PAD_X        4
#define UI_VLAN_PAGER_Y          210
#define UI_VLAN_DOT_SIZE         4
#define UI_VLAN_DOT_GAP          8
#define UI_VLAN_SWIPE_Y          55
#define UI_VLAN_SWIPE_H          175

#define UI_COLOR_TEXT            0xFFFFFF
#define UI_COLOR_TEXT_INACTIVE   0xA3A3A3
#define UI_COLOR_VLAN_ACCENT     0x10B981
#define UI_COLOR_PAGER_ACTIVE    0xFFFFFF
#define UI_COLOR_PAGER_INACTIVE  0xA3A3A3

#define UI_SWIPE_H_THRESHOLD     30
#define UI_SWIPE_DOWN_THRESHOLD  50
#define UI_SWIPE_TOP_ZONE_Y      20

#endif
