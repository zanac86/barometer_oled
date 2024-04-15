#include "plotter.h"
#include "history.h"
#include "OLED.h"
#include "main.h"

uint8_t display_mode = 0;

void change_display_mode()
{
	display_mode = (display_mode + 1) % 2;
}

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

void draw_p_t()
{
	float p1 = ((float) now_pressure) / 100.0;
	float p2 = ((float) now_pressure * 100.0) / 13332.2;
	float t = ((float) now_temperature / 100.0);
	char buf1[30];

	sprintf(buf1, "%6.1f", p1);
	OLED_DrawStrZoom(buf1, OLED_LEFT, 2, 1, 2);

	sprintf(buf1, "%3.0f", p2);
	OLED_DrawStrZoom(buf1, OLED_RIGHT, 2, 1, 2);

	sprintf(buf1, "hPa    %4.1fCC    mmHg", t);
	*(uint8_t*) (buf1 + 11) = 0xb8;
	OLED_DrawStrZoom(buf1, OLED_CENTER, 24, 1, 1);
}

#define LIMIT(x,a,b) ((x<a)?a:((x>b)?b:x))

void draw_chart()
{
	for (uint8_t x = 0; x < 5; x++)
	{
		uint8_t xx = 3 + x * 21;
		for (uint8_t y = 0; y < 8; y++)
		{
			OLED_DrawPixel(xx, 0 + y * 4);
		}
	}
	OLED_DrawLine(3, 31, 87, 31);

	uint8_t h_max = 31;
	uint8_t y0 = 0;
	normalize_history(h_max);

	for (uint8_t i = 0; i < HISTORY_LEN; i++)
	{
		// каждый столбик 3 пикс, пробел 2 пикс
		uint8_t x = 5 + i * 5 + (i / 4);
		// для вертикальной линии y2 должен быть на 1 больше
		// при рисовании там условие <(y2-y1)
		if (normalized_history[i] == 0)
		{
			continue;
		}
		uint8_t y = y0 + (h_max - normalized_history[i]);

		OLED_DrawLine(x, y, x, 31);
		OLED_DrawLine(x + 1, y, x + 1, 31);
		OLED_DrawLine(x + 2, y, x + 2, 31);
	}

	char buf1[30];
	uint32_t p = (uint32_t) (((float) now_pressure * 100.0) / 13332.2);
	sprintf(buf1, "%lu", p);
	OLED_DrawStrZoom(buf1, 91, 2, 1, 2);
	sprintf(buf1, "mmHg");
	OLED_DrawStrZoom(buf1, OLED_RIGHT, 24, 1, 1);

}

void display()
{
	OLED_Clear(0);
	if (display_mode == 0)
	{
		draw_p_t();
	}
	else
	{
		draw_chart();
	}
	OLED_UpdateScreen();
}
