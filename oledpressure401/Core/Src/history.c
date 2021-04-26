#include "history.h"

int32_t History[HISTORY_LEN];
int32_t now_pressure;

uint16_t first_push = 1;

void init_history()
{
	now_pressure = 0;
	for (uint16_t i = 0; i < HISTORY_LEN; i++)
	{
		History[i] = 0;
	}
}

int32_t abs(int32_t a)
{
	if (a < 0)
		return -a;
	return a;
}

int32_t get_trend(uint16_t start_index, uint16_t len, int32_t *p_min,
		int32_t *p_max)
{
	if ((start_index + len) > HISTORY_LEN)
		return 0;
	uint16_t index_min = start_index;
	uint16_t index_max = start_index;
	int32_t value_min = History[start_index];
	int32_t value_max = History[start_index];
	uint16_t len_end = start_index + len;
	for (uint16_t i = start_index; i < len_end; i++)
	{
		if (History[i] > value_max)
		{
			value_max = History[i];
			index_max = i;
		}
		if (History[i] < value_min)
		{
			value_min = History[i];
			index_min = i;
		}
	}
	/*
	 *   100654->1006hPa
	 * если диапазон min-max меньше < 0.11mm в кодах hPa*100/13332.2,
	 * то изменения нет.
	 * более старые значения - с большим индексом
	 * значит если индекс минимума меньше индекса максимума
	 * то это уменьшение (-)
	 */

	*p_min = value_min;
	*p_max = value_max;
	int32_t range = abs(value_max - value_min);
	if (range < 20)
		return 0;
	if (index_min < index_max)
		return -range;
	if (index_min > index_max)
		return range;
	return 0;
}

void get_range(int32_t *p_min, int32_t *p_max)
{
	int32_t value_min = History[0];
	int32_t value_max = History[0];
	int32_t value_base = 0;
	for (uint16_t i = 0; i < HISTORY_LEN; i++)
	{
		value_base += History[i];
		if (History[i] > value_max)
		{
			value_max = History[i];
		}
		if (History[i] < value_min)
		{
			value_min = History[i];
		}
	}

//	int32_t base = (value_base / HISTORY_LEN);

	if ((value_max - value_min) < 20)
	{
		value_min -= 50;
		value_max += 50;
	}
	else
	{
		value_min -= 20;
		value_max += 20;
//		int32_t d_min = p_base - p_min;
//		int32_t d_max = p_max - p_base;
//		int32_t d = (d_max > d_min) ? d_max : d_min;
//		p_min = p_base - d;
//		p_max = p_base + d;
	}

	*p_min = value_min;
	*p_max = value_max;
}

void push(int32_t new_value)
{
	if (first_push == 1)
	{
		for (uint16_t i = 0; i < HISTORY_LEN; i++)
		{
			History[i] = new_value;
		}
		first_push = 0;
		return;
	}
	for (uint16_t i = (HISTORY_LEN - 1); i > 0; i--)
	{
		History[i] = History[i - 1];
	}
	History[0] = new_value;
}

void append_measure_to_history(int32_t pressure)
{
	push(pressure); // get a raw value
}

void set_now_pressure(int32_t pressure)
{
	now_pressure = pressure;
}
