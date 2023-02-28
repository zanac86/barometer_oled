#include "plotter.h"
#include "history.h"
#include "OLED.h"
#include "main.h"

// input pressure hPa x 100 = 100000 => 750 mmHg
void draw_pressure_mmhg(int32_t p_hpa, uint8_t x, uint8_t y, uint8_t zoom)
{
	int32_t p_mmhg = (p_hpa * 100) / 13332; // 13332.2
	char buf[15];
	sprintf(buf, "%3ld", p_mmhg);
	OLED_DrawStrZoom(buf, x, y, 1, zoom);
}

void draw_pressure_mmhg_float(int32_t p_hpa, uint8_t x, uint8_t y)
{
	float p_mmhg = ((float) p_hpa * 100.0) / 13332.2; // 13332.2
	char buf[15];
	sprintf(buf, "%5.1f", p_mmhg);
	OLED_DrawStrZoom(buf, x, y, 1, 1);
}

void draw_pressure_mmhg_big()
{
	float p_mmhg = ((float) now_pressure * 100.0) / 13332.2;
	char buf[15];
	sprintf(buf, "%5.1f", p_mmhg);
	OLED_DrawStrZoom(buf, OLED_CENTER, OLED_CENTER, 1, 4);
}

void draw_pressure_small()
{
	float p_mmhg = ((float) now_pressure * 100.0) / 13332.2;
	float p_hpa = ((float) now_pressure) / 100.0;
	char buf1[15];
	char buf2[15];
	sprintf(buf1, "%5.1f mmHg", p_mmhg);
	sprintf(buf2, "%6.2f hPa", p_hpa);
	if (OLED_HEIGHT == 32)
	{
		OLED_DrawStrZoom(buf1, OLED_CENTER, 0, 1, 2);
		OLED_DrawStrZoom(buf2, OLED_CENTER, 24, 1, 1);
	}
	else
	{
		OLED_DrawStrZoom(buf1, OLED_CENTER, 8, 1, 2);
		OLED_DrawStrZoom(buf2, OLED_CENTER, 48, 1, 1);
	}
}

#define LIMIT(x,a,b) ((x<a)?a:((x>b)?b:x))

void draw_chart()
{
	uint16_t x0=0;
	int32_t p_min;
	int32_t p_max;
	get_range(&p_min, &p_max);


	for(uint16_t i=0;i<7;i++)
	{
		for(uint16_t y=0;y<10;y++)
		{
			OLED_DrawPixel(x0+i*3*4, y*3);
		}
	}
	OLED_DrawHLine(x0, 31, 72, 1);


	float pmin = (float) p_min;
	float pmax = (float) p_max;

	float kh = (float) (OLED_HEIGHT - 2) / (pmax - pmin);
	for (uint16_t i = 0; i < HISTORY_LEN; i++)
	{
		uint16_t x = i * 3;
		float p = (float) History[HISTORY_LEN - 1 - i];
		float v = LIMIT(p, pmin, pmax);
		v = kh * (v - pmin);
		v = (v < 1) ? 1 : v;
		uint8_t h = (uint8_t) v;
		uint8_t y = (OLED_HEIGHT - 1) - h;
		OLED_DrawVLine(x0+x, y, h);
		OLED_DrawVLine(x0+x+1, y, h);
	}


	draw_pressure_mmhg_float(p_max, 94, 0);
	draw_pressure_mmhg_float(p_min, 94, 25);


//	// давление в мм. рт. ст - с один знаком 746.1
//	draw_pressure_mmhg_float(p_max, 0, 0);
//	draw_pressure_mmhg_float(History[0], 98, 12);
//	draw_pressure_mmhg_float(p_min, 0, 25);

//	char buf[15];
//	sprintf(buf, "%5ld", p_min);
//	OLED_DrawStr(buf, 98, 40, 1);
//	sprintf(buf, "%5ld", p_max);
//	OLED_DrawStr(buf, 98, 48, 1);
//	sprintf(buf, "%5ld", p_max-p_min);
//	OLED_DrawStr(buf, 98, 56, 1);

	// давление в Па - знаков без запятой - 101612 или 99912
//	char str[15];
//	sprintf(str, "%6ld", p_max);
//	OLED_DrawStr(str, 92, 0, 1);
//	sprintf(str, "%6ld", History[0]);
//	OLED_DrawStr(str, 92, 8, 0);
//	sprintf(str, "%6ld", p_base);
//	OLED_DrawStr(str, 92, 16, 1);
//	sprintf(str, "%6ld", p_min);
//	OLED_DrawStr(str, 92, 24, 1);
}

void draw_numbers()
{
	float p_mmhg = ((float) now_pressure * 100.0) / 13332.2;
//	float p_hpa = ((float) now_pressure) / 100.0;
	char buf[8];
	sprintf(buf, "%4.0f", p_mmhg);
	OLED_DrawStrZoom(buf, 80, 9, 1, 2);
//	sprintf(buf, "%4.0f", p_hpa);
//	OLED_DrawStrZoom(buf, 80, 18, 1, 2);



}

void display(uint16_t full_redraw)
{
	if (full_redraw)
	{
		OLED_Clear(0);
		draw_chart();
		draw_numbers();
		OLED_UpdateScreen();
	}
	else
	{
//		draw_numbers();
//		OLED_UpdateScreen();
	}

}
