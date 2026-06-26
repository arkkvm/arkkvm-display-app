#ifndef NETWORK_VLAN_H
#define NETWORK_VLAN_H

#include "page_manager.h"
#include "../components/view_network_info.h"
#include <stdbool.h>

void network_vlan_clear(GlobalData * data);
bool network_vlan_apply_report(GlobalData * data, const char * json);
bool network_vlan_apply_ipv6_report(GlobalData * data, const char * json);
void network_vlan_refresh_addresses(GlobalData * data);
void network_vlan_fill_state(const GlobalData * data, NetworkInfoState * state);

#endif
