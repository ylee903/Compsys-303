// =============================================
// File: Pacemaker_C.h  (STUB — link‑safe)
// Minimal API so main.c links even when C‑mode is not implemented yet.
// =============================================
#ifndef PACEMAKER_C_H
#define PACEMAKER_C_H

#ifdef __cplusplus
extern "C" {
#endif

// Opaque-ish state; main only needs the type name
typedef struct { int _unused; } PacemakerC;

// Lifecycle
void PMc_init(PacemakerC* s);
void PMc_set_led_pulse_ms(PacemakerC* s, int ms);

// 1 ms ISR start — return 0 to indicate OK in stub
int  PMc_start_1ms_alarm(void);

// Inputs / stepping
void PMc_set_senses(PacemakerC* s, int AS_raw, int VS_raw);
void PMc_run_for_elapsed_ms(PacemakerC* s);

// Outputs / status
void PMc_poll_and_clear_pulses(PacemakerC* s, int* AP_any, int* VP_any);
int  PMc_led_AP_on(const PacemakerC* s);
int  PMc_led_VP_on(const PacemakerC* s);

#ifdef __cplusplus
}
#endif

#endif // PACEMAKER_C_H


// =============================================
// File: Pacemaker_C.c  (STUB — link‑safe)
// Provides do‑nothing definitions so the project links.
// =============================================
#include "Pacemaker_C.h"

void PMc_init(PacemakerC* s) { (void)s; }
void PMc_set_led_pulse_ms(PacemakerC* s, int ms) { (void)s; (void)ms; }
int  PMc_start_1ms_alarm(void) { return 0; }
void PMc_set_senses(PacemakerC* s, int AS_raw, int VS_raw) { (void)s; (void)AS_raw; (void)VS_raw; }
void PMc_run_for_elapsed_ms(PacemakerC* s) { (void)s; }
void PMc_poll_and_clear_pulses(PacemakerC* s, int* AP_any, int* VP_any) { if (AP_any) *AP_any = 0; if (VP_any) *VP_any = 0; }
int  PMc_led_AP_on(const PacemakerC* s) { (void)s; return 0; }
int  PMc_led_VP_on(const PacemakerC* s) { (void)s; return 0; }
