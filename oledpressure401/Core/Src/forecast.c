#include "forecast.h"
#include "plotter.h"
#include "history.h"
#include "OLED.h"
#include "main.h"

/*
 dP/dt >  250Pa/h     Unstable high pressure system
 50Pa/h < dP/dt <  250Pa/h     for some time Stable good weather
 -50Pa/h < dP/dt <   50Pa/h     for some time Stable weather
 -250Pa/h < dP/dt <  -50Pa/h     for some time Stable rainy weather
 dP/dt < -250Pa/h     Unstable high pressure system

 Look at the pressure change over3 hours
 If pressure is descending, then a low pressure area is approaching
 If pressure is ascending , then a low is passing or a high pressure is coming
 When pressure is changing rapidly (>6hPa/3 hours), it will be windy (or potentially windy)

 More detailed:
 Pressure falling slowly (0.5 - 3 hPa in 3h): low is weak, dying or moving slowly. You might get some rain but typically no high winds.
 Pressure falling moderately (3-6 hPa/3h): rapid movement or deepening low. Moderate winds and rain and a warm front.
 : the low is passing fast, the day after tomorrow will typically be fine.
 Pressure falling fast (6-12 hPa/3h) : Storm conditions highly likely.
 Pressure rises are connected with gradually drier weather

 • Если атмосферное давление держится не очень высоко — 750 — 740 мм, наблюдается его неравномерное понижение: то быстрее,
 то медленнее; иногда может быть даже кратковременное незначительное повышение с последующим падением, — это указывает
 на прохождение циклона. Распространенное заблуждение гласит, что циклон всегда несет с собой ненастье. На самом деле,
 погода в циклоне очень неоднородна, — порой небо остается абсолютно безоблачным и циклон уходит, так и не пролив ни капли дождя.
 Более существенно не сам факт низкого давления, а его постепенное падение. Низкое атмосферное давление само по себе
 еще не признак ненастья.

 • Если давление очень быстро падает до 740 или даже 730 мм, — это обещает короткое, но бурное ненастье, которое будет
 продолжаться некоторое время и при повышении давления. Чем быстрее падает давление, тем продолжительнее
 будет неустойчивая погода; возможно наступление длительного ненастья.

 Признаки улучшения погоды

 • Рост давления воздуха говорит о близящемся улучшении погоды, особенно если он начался после длительного периода низкого давления.

 • Повышение атмосферного давления при наличии тумана, — указывает на улучшение погоды.

 Признаки сохранения хорошей погоды

 • Если барометрическое давление медленно поднимается в течение нескольких дней или остается без изменения
 при южном ветре, — это признак сохранения хорошей погоды.

 • Если барометрическое давление повышается при сильном ветре — признак сохранения хорошей погоды.
 */

uint16_t windy;
PRESSURE_TREND w6;
PRESSURE_TREND w3;
WEATHER weather;
int32_t d3;
int32_t d6;

#define av3p(a, i) ((a[i]+a[i+1]+a[i+2])/3)

void estimate_trends()
{
	//	         |            |           |           |
	//	15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
	//     ^^^^^^^^                 ^^^^^^^     ^^^^^^^
	// for 15min - 0, 4, 12
	// for 10min - 0, 6, 18

	uint16_t n_per_h = (SECONDS_PER_HOUR / SECONDS_UPDATE_HISTORY); // 3600/900(15мин) или 3600/600(10мин)
	int32_t p0h = av3p(History, 0);
	int32_t p3h = av3p(History, n_per_h * 3);
	int32_t p6h = av3p(History, n_per_h * 6);
	d3 = p0h - p3h;
	d6 = p3h - p6h;
	if (abs(d6) < 50)
	{
		w6 = PRESSURE_STABLE;
	}
	else
	{
		w6 = (d6 < 0) ? PRESSURE_FALLING : PRESSURE_RISING;
	}
	if (abs(d3) < 50)
	{
		w3 = PRESSURE_STABLE;
	}
	else
	{
		w3 = (d3 < 0) ? PRESSURE_FALLING : PRESSURE_RISING;
	}
	windy = 0;
	if (abs(d3) > 600)
	{
		windy = 1;
	}
}

void test_forecast_weather()
{
	estimate_trends();
	weather = WEATHER_NONE;
	switch (w3)
	{
	case PRESSURE_RISING:
		switch (w6)
		{
		case PRESSURE_RISING:
			weather = WEATHER_SUN;
			break;
		case PRESSURE_STABLE:
			weather = WEATHER_SUN;
			break;
		case PRESSURE_FALLING:
			weather = WEATHER_CLOUD_SUN;
			break;
		}
		break;
	case PRESSURE_STABLE:
		switch (w6)
		{
		case PRESSURE_RISING:
			weather = WEATHER_SUN;
			break;
		case PRESSURE_STABLE:
			weather = WEATHER_SUN;
			break;
		case PRESSURE_FALLING:
			weather = WEATHER_CLOUD;
			break;
		}
		break;
	case PRESSURE_FALLING:
		switch (w6)
		{
		case PRESSURE_RISING:
			weather = WEATHER_CLOUD;
			break;
		case PRESSURE_STABLE:
			weather = WEATHER_RAIN;
			break;
		case PRESSURE_FALLING:
			weather = WEATHER_RAIN;
			break;
		}
		break;
	}
}

WEATHER make_forecast(int32_t *_d3, int32_t *_d6)
{
	test_forecast_weather();
	*_d3 = d3;
	*_d6 = d6;
	return weather;
}
