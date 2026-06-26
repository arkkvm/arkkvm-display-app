// components/view_net_dis.h
#ifndef VIEW_NET_DIS_H
#define VIEW_NET_DIS_H
#include "../../lvgl/lvgl.h"
#include <stdbool.h>

void view_net_dis_init(lv_obj_t* parent);
void view_net_dis_set_visible(bool visible);
void update_view_data();
lv_obj_t* get_view_net_dis_container(void);  
#endif