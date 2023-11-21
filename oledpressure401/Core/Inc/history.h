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
#define HISTORY_LEN 16
// Main history buffer
extern int32_t pressure_history[HISTORY_LEN];
extern uint8_t normalized_history[HISTORY_LEN];

// Only diffs
extern int32_t now_pressure;
extern int32_t now_temperature;

/// clear history arrays;
void init_history();
/// pressure*100 = 100654 = 1006HPa 
/// convert 100654/133.322 = 754.96 mmHg
/// temperature*100 = 2734 = 27.34 °C
void append_measure_to_history(int32_t pressure, int32_t temperature);
void set_now_pressure(int32_t pressure, int32_t temperature);

/// заполнить массив для графика
void normalize_history(uint8_t max_h_norm);

#endif /* HISTORY_H_ */
