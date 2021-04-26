#include "plotter.h"
#include "history.h"
#include "OLED.h"
#include "main.h"
#include "forecast.h"

//void draw_real_hist()
//{
//	uint8_t min_h = 1;
//	uint8_t max_h = OLED_HEIGHT - 1; // 31
//
//	int32_t K = (1000 * (int32_t) max_h) / history_minmax.range;
//	int32_t min_value = history_minmax.min;
//
//	uint8_t line_step = 2;
//
//	for (uint16_t i = 0; i < HISTORY_LEN; i++)
//	{
//		int32_t value = History[HISTORY_LEN - 1 - i];
//		int32_t p = (int32_t) ((K * (value - min_value))) / 1000;
//		uint8_t h = p;
//		if (h < min_h)
//			h = min_h;
//		if (h > max_h)
//			h = max_h;
//		uint8_t x = i * line_step;
//		uint8_t y = max_h - h;
//		OLED_DrawPixel(x, y);
//		OLED_DrawPixel(x + 1, y);
//	}
//}

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

void draw_trends()
{
	/*
	 48 points with interval 15 min => 720 min = 12 hours
	 group to 3 hours - 12 points -> 4 groups
	 6*3+3=21px px for 1 group
	 0px: 4 groups -> 84px

	 84px: 8px -> space

	 92px: currP -> zoom=2 -> 36px
	 */

	uint16_t sx = 22;
	uint16_t group_len = RECORDS_PER_TREND_INTERVAL;
	draw_pressure_mmhg(now_pressure, 92, 2, 2);

	int16_t xbm_y = OLED_HEIGHT / 2 - 4; // icon height  = 8

	for (uint16_t i = 0; i < 4; i++)
	{
		uint16_t ix = (3 - i) * sx; // слева старые, правее - новые значения
		uint16_t n0 = i * group_len;
		int32_t p_min, p_max;
		int32_t dp = get_trend(n0, group_len, &p_min, &p_max);
		if (dp == 0)
		{
			draw_pressure_mmhg((p_max + p_min) / 2, ix, OLED_CENTER, 1);
		}
		else
		{
			draw_pressure_mmhg(p_max, ix, xbm_y - 4 - 8, 1); // -interval -font height
			draw_pressure_mmhg(p_min, ix, xbm_y + 8 + 4, 1); // +icon height +interval
			// должно быть наоборот, так как самые старые в конце массива
			// надо подумать
			if (dp > 0)
			{
				OLED_DrawXBM(ix + 6, xbm_y, arr_up);
			}
			else
			{
				OLED_DrawXBM(ix + 6, xbm_y, arr_dn);
			}
		}
		OLED_DrawVLine(ix + sx - 3, 0, OLED_HEIGHT);
	}

	char str[15];
	uint16_t mins = (SECONDS_UPDATE_HISTORY / 60);
	uint16_t hours = (SECONDS_UPDATE_HISTORY * RECORDS_PER_TREND_INTERVAL)
			/ (SECONDS_PER_HOUR);
	sprintf(str, "%dm %dH", mins, hours);
	OLED_DrawStr(str, 92, 24, 1);

}

#define LIMIT(x,a,b) ((x<a)?a:((x>b)?b:x))

void draw_chart()
{
	int32_t p_min;
	int32_t p_max;
	get_range(&p_min, &p_max);

	float pmin = (float) p_min;
	float pmax = (float) p_max;

	float kh = (float)(OLED_HEIGHT-2) / (pmax - pmin);
	for (uint16_t i = 0; i < HISTORY_LEN; i++)
	{
		uint16_t x = i * 2;
		float p = (float) History[HISTORY_LEN - 1 - i];
		float v = LIMIT(p, pmin, pmax);
		v = kh * (v - pmin);
		v = (v < 1) ? 1 : v;
		uint8_t h = (uint8_t) v;
		uint8_t y = (OLED_HEIGHT - 1) - h;
		if (y == 32)
		{
			OLED_DrawVLine(x, y, h);
		}
		OLED_DrawVLine(x, y, h);
	}
//	draw_pressure_mmhg(History[0], 106, OLED_CENTER, 1);
// пробел в начале строк с давлением затирает два первых отсчета

	// давление в мм. рт. ст - с один знаком 746.1
	draw_pressure_mmhg_float(p_max, 98, 0);
	draw_pressure_mmhg_float(History[0], 98, 12);
	draw_pressure_mmhg_float(p_min, 98, 24);

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

void draw_forecast()
{
	int32_t d3;
	int32_t d6;
	WEATHER w = make_forecast(&d3, &d6);
	uint8_t x = 16;
	uint8_t y = 0;
	switch (w)
	{
	case WEATHER_CLOUD:
		OLED_DrawXBM(x, y, cloud_bits);
		break;
	case WEATHER_CLOUD_SUN:
		OLED_DrawXBM(x, y, cloud_sun_bits);
		break;
	case WEATHER_RAIN:
		OLED_DrawXBM(x, y, rain_bits);
		break;
	case WEATHER_SUN:
		OLED_DrawXBM(x, y, sun_bits);
		break;
	default:
		break;
	}

	char str[15];
	sprintf(str, "%6ld", d3);
	OLED_DrawStr(str, 92, 0, 1);
	sprintf(str, "%6ld", d6);
	OLED_DrawStr(str, 92, 10, 1);
}

void display(uint8_t display_type)
{
	OLED_Clear(0);

//	display_type = DISPLAY_CHART;

	switch (display_type)
	{
	case DISPLAY_BIG:
		draw_pressure_mmhg_big();
		break;
	case DISPLAY_SMALL:
		draw_pressure_small();
		break;
	case DISPLAY_TRENDS:
		draw_trends();
		break;
	case DISPLAY_CHART:
		draw_chart();
		break;
//	case DISPLAY_FORECAST:
//		draw_forecast();
//		break;
	}
//	OLED_DrawRectangle(0, 0, OLED_WIDTH - 1, OLED_HEIGHT - 1);

	OLED_UpdateScreen();
}
