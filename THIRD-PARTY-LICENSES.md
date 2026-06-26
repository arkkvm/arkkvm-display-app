# Third-Party Licenses

The `arkkvm_ui` binary statically links the components below. Each component
remains under its own license when distributed separately. MIT-licensed
components are included under MIT and are compatible with GPL-2.0-or-later.

| Component | License | Location |
|-----------|---------|----------|
| LVGL | MIT | [lvgl/LICENCE.txt](lvgl/LICENCE.txt) |
| lv_drivers | MIT | [lv_drivers/LICENSE](lv_drivers/LICENSE) |
| cJSON | MIT | [src/utils/cJSON.c](src/utils/cJSON.c) (file header) |
| Funnel Sans | OFL-1.1 | [font/funnel_sans/OFL.txt](font/funnel_sans/OFL.txt) |

## LVGL

Copyright (c) 2021 LVGL Kft

See [lvgl/LICENCE.txt](lvgl/LICENCE.txt) for the full MIT license text.

### Bundled components (compiled into `arkkvm_ui`)

The following third-party code is included inside the LVGL tree and linked
into the binary:

| Component | License | Copyright | Location |
|-----------|---------|-----------|----------|
| tlsf | BSD-like | Copyright (c) 2006-2016, Matthew Conte | [lvgl/src/misc/lv_tlsf.c](lvgl/src/misc/lv_tlsf.c) |
| lv_printf | MIT | Copyright (c) Marco Paland, 2014-2019 | [lvgl/src/misc/lv_printf.c](lvgl/src/misc/lv_printf.c) |

## lv_drivers

Copyright (c) 2019 LittlevGL

See [lv_drivers/LICENSE](lv_drivers/LICENSE) for the full MIT license text.

## cJSON

Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

See the license notice in [src/utils/cJSON.c](src/utils/cJSON.c) and
[src/utils/cJSON.h](src/utils/cJSON.h).

## Funnel Sans

Copyright 2023 The Funnel Project Authors
(https://github.com/Dicotype/Funnel)

Licensed under the SIL Open Font License, Version 1.1.
See [font/funnel_sans/OFL.txt](font/funnel_sans/OFL.txt) for the full license text.

Embedded font glyph data is included in [font/funnel_sans/](font/funnel_sans/).
