#ifndef PACEMAKER_C_H
#define PACEMAKER_C_H

#include <alt_types.h>
#include "timing.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int dbg_period_ms;   // toggle period (default 1000ms)
  int dbg_accum_ms;    // elapsed accumulator
  int dbg_led7;        // 0/1 desired state for LEDG7
} PacemakerC;

// Lifecycle
void PMc_init(PacemakerC* s);
void PMc_set_led_pulse_ms(PacemakerC* s, int ms); // placeholder (no-op)
int  PMc_start_1ms_alarm(void);                   // stub: return 0

// Inputs / stepping (stubs for now)
void PMc_set_senses(PacemakerC* s, int AS_raw, int VS_raw);
void PMc_run_for_elapsed_ms(PacemakerC* s);

// Outputs / status (stubs)
void PMc_poll_and_clear_pulses(PacemakerC* s, int* AP_any, int* VP_any);
int  PMc_led_AP_on(const PacemakerC* s);
int  PMc_led_VP_on(const PacemakerC* s);

// Debug blinker API (LEDG7)
void PMc_debug_set_blink_period_ms(PacemakerC* s, int period_ms);
void PMc_debug_tick_elapsed_ms(PacemakerC* s, int elapsed_ms);
int  PMc_debug_led7(const PacemakerC* s);

#ifdef __cplusplus
}
#endif

#endif // PACEMAKER_C_H
