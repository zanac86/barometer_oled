#include "history.h"

int32_t pressure_history[HISTORY_LEN];
uint8_t normalized_history[HISTORY_LEN];
int32_t now_pressure;
int32_t now_temperature;

void init_history()
{
	now_pressure = 0;
	now_temperature = 0;
	for (uint16_t i = 0; i < HISTORY_LEN; i++)
	{
		pressure_history[i] = 0;
		normalized_history[i] = 0;
	}
}

void normalize_history(uint8_t max_h_norm)
{
	int32_t mn = pressure_history[HISTORY_LEN - 1];
	int32_t mx = pressure_history[HISTORY_LEN - 1];
	// find min/max == range
	for (uint8_t i = 0; i < HISTORY_LEN; i++)
	{
		normalized_history[i] = 0;
		if (pressure_history[i] == 0)
		{
			// 0 значений давления не должно быть и не учитывать
			// они и рисоваться не будут
			continue;
		}
		if (pressure_history[i] < mn)
		{
			mn = pressure_history[i];
		}
		if (pressure_history[i] > mx)
		{
			mx = pressure_history[i];
		}
	}

	if (((mn == 0) || (mx == 0)))
	{
		// все нули и рисовать нечего
		return;
	}

	if (mn == mx)
	{
		// если менялось мало и в целых значениях изменения нет
		// то вернем середину от нормировки
		for (uint8_t i = 0; i < HISTORY_LEN; i++)
		{
			if (pressure_history[i] != 0)
			{
				normalized_history[i] = max_h_norm / 2;
			}
		}
		return;
	}

	// диапазон изменения истории
	int32_t range = mx - mn;

	// +- 2 это в 20, так как в истории лежат  в kPa*100
	if (range <= 250)
	{
		mn = mn - 250;
		mx = mx + 250;
		range = mx - mn;
	}

	for (uint8_t i = 0; i < HISTORY_LEN; i++)
	{
		if (pressure_history[i] == 0)
		{
			continue;
		}
		int32_t tmp = (int32_t) (pressure_history[i] - mn);
		tmp = (tmp * max_h_norm) << 10;
		tmp = (tmp / (range + 1));
		tmp = (tmp >> 10);
		if (tmp == 0)
		{
			tmp = 1;
		}
		normalized_history[i] = (uint8_t) tmp;
	}
}

void push(int32_t new_value)
{
	// самое старое в начале, так проще рисовать
	// а новое положить в последний индекс
	for (uint16_t i = 0; i < (HISTORY_LEN - 1); i++)
	{
		pressure_history[i] = pressure_history[i + 1];
	}
	pressure_history[HISTORY_LEN - 1] = new_value;

//     тест
//     const uint16_t u[] = {742, 740, 747, 750, 752, 753, 755, 757, 763, 762, 759, 757, 753, 750, 748, 770 };
//     const uint16_t u[] = {750, 751, 751, 751, 750, 750, 750, 750, 749, 750, 750, 750, 750, 750, 751, 761 };
//     for (uint8_t i = 0; i < HISTORY_LEN; i++)
//     {
//         pressure_history[i] = (int32_t)((float)u[i]*133.322);
//     }

}

void append_measure_to_history(int32_t pressure, int32_t temperature)
{
	push(pressure); // store raw value
}

void set_now_pressure(int32_t pressure, int32_t temperature)
{
	now_pressure = pressure;
	now_temperature = temperature;
}
