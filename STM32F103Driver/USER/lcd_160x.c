#include "bios_dir.h"
#include "lcd_160x.h"

#if (LCD10_DISP)//为10字符带状态标签显示屏
#include "lcd_ht1621.c"
#elif (DD_LCD_16021)
#include "lcd_160x21.c"
#elif (DD_LCD_1601)
#include "lcd_160x1.c"
#elif (DD_ZIP)
#include "lcd_160x2.c"
#endif

