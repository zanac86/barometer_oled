#ifndef HISTORY_H_
#define HISTORY_H_

#include <stdint.h>

// Pressure valid range 933 hPa (700 mmHg) ... 1066 hPa (800 mmHg)
#define MIN_PRESSURE  93300
#define MAX_PRESSURE 106600

/*
 History stores
 P,T - sample time = 15 min
 Total 48 records in memory = 12 hours

 3 hours - 12 records - trend


 */

// количество записей для интервала расчета тренда
#define RECORDS_PER_TREND_INTERVAL 12

#define HISTORY_LEN 48
// Main history buffer
extern int32_t History[HISTORY_LEN];
// Only diffs
extern int32_t now_pressure;

/// clear history arrays;
void init_history();
/// pressure*100 = 100654 = 1006HPa 
/// convert 100654/133.322 = 754.96 mmHg
/// temperature*100 = 2734 = 27.34 °C
void append_measure_to_history(int32_t pressure);
void set_now_pressure(int32_t pressure);
void forecast();

/*
 * 1. min-max range < +-2mm 133 -> no change 0
 * 2. index-min < index-max -> go up  +range
 * 3. else -> go down -range
 */
int32_t get_trend(uint16_t start_index, uint16_t len, int32_t *p_min,
		int32_t *p_max);

void get_range(int32_t *p_min, int32_t *p_max);

int32_t abs(int32_t a);

#endif /* HISTORY_H_ */
