#ifndef PLOTTER_H_
#define PLOTTER_H_

#include <stdint.h>

#define DISPLAY_BIG 0
#define DISPLAY_SMALL 1
#define DISPLAY_TRENDS 2
#define DISPLAY_CHART 3
//#define DISPLAY_FORECAST 4

#define DISPLAY_MODES_COUNT 4

// main entry for display screens
void display(uint8_t display_type);

// just print 3-digit pressure hPa x 100 = 100000 = 750 mmHg
void draw_pressure_mmhg(int32_t p_hpa, uint8_t x, uint8_t y, uint8_t zoom);
// just print 3-digit pressure hPa x 100 = 100000 = 750 mmHg
void draw_pressure_mmhg_float(int32_t p_hpa, uint8_t x, uint8_t y);

// большие цифры
void draw_pressure_mmhg_big();

// маленькие цифры
void draw_pressure_small();

// изменения по 3 часа - 4 поля - в сумме за 12 часов
void draw_trends();

// график 48 точек
void draw_chart();

void draw_forecast();


#endif /* PLOTTER_H_ */
