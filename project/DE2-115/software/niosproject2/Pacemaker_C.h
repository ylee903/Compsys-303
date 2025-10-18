// =============================================
// File: Pacemaker_C.h
// =============================================
#ifndef PACEMAKER_C_H
#define PACEMAKER_C_H


#include <alt_types.h>
#include "timing.h" // AVI_VALUE, AEI_VALUE, etc.


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
int AS_raw, VS_raw; // sensed inputs for current ms
int AS, VS; // gated by PVARP/VRP
int AP, VP; // one‑ms outputs
int AP_fired, VP_fired; // sticky flags for UART/LED reporting
int AVI, AEI, PVARP, VRP, LRI, URI; // ms counters
int seen_AS_since_last_V;
int vp_waiting_for_URI;
int led_pulse_ms;
int AP_led_ms, VP_led_ms; // LED stretch timers
} PacemakerC;


void PMc_init(PacemakerC* s);
void PMc_set_led_pulse_ms(PacemakerC* s, int ms);
int PMc_start_1ms_alarm(void);
void PMc_set_senses(PacemakerC* s, int AS_raw, int VS_raw);
void PMc_run_for_elapsed_ms(PacemakerC* s);
void PMc_poll_and_clear_pulses(PacemakerC* s, int* AP_any, int* VP_any);
int PMc_led_AP_on(const PacemakerC* s);
int PMc_led_VP_on(const PacemakerC* s);


#ifdef __cplusplus
}
#endif


#endif // PACEMAKER_C_H
