#ifndef FORECAST_H_
#define FORECAST_H_

#include <stdint.h>

typedef enum
{
	WEATHER_NONE,
	WEATHER_SUN,
	WEATHER_RAIN,
	WEATHER_CLOUD,
	WEATHER_CLOUD_SUN,
} WEATHER;

typedef enum
{
	PRESSURE_STABLE,
	PRESSURE_FALLING,
	PRESSURE_RISING
} PRESSURE_TREND;

WEATHER make_forecast(int32_t *_d3, int32_t *_d6);

#endif /* FORECAST_H_ */
