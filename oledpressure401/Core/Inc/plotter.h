#ifndef PLOTTER_H_
#define PLOTTER_H_

#include <stdint.h>

// main entry for display screens
void display(uint16_t full_redraw);

// just print 3-digit pressure hPa x 100 = 100000 = 750 mmHg
void draw_pressure_mmhg(int32_t p_hpa, uint8_t x, uint8_t y, uint8_t zoom);
// just print 3-digit pressure hPa x 100 = 100000 = 750 mmHg
void draw_pressure_mmhg_float(int32_t p_hpa, uint8_t x, uint8_t y);

// большие цифры
void draw_pressure_mmhg_big();

// маленькие цифры
void draw_pressure_small();

// график 48 точек
void draw_chart();

#endif /* PLOTTER_H_ */
