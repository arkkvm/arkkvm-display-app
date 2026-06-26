#ifndef WEL_PAGE_H
#define WEL_PAGE_H

#include "../core/page_manager.h"
#include <stdbool.h>

typedef struct {
     lv_obj_t* container;
     lv_obj_t* state_label;
} WelPageUI;

void wel_page_init(lv_obj_t* container);
void wel_page_update(void);
void wel_page_cleanup(void);


// TODO: implement .c based on the page structure below
/**
 * Welcome page
 * {
 * view:[
 *  {
 *  "type": "icon",
 * "src": "icon_wel_logo",
 * "position": {"x": 62, "y": 100},
 * "size": {"width": 160, "height": 30}
 * },{
 *  "type":"label",
 *  "name":"state_label",
 *  "font_size":14,
 *  "position": "centered horizontally, 184px from top",
 *  "text":"Starting..."
 * 
 * }
 * ]
 * 
 * }
 */

#endif // WEL_PAGE_H