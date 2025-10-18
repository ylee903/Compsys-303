// ======================= Pacemaker_C.h =======================
// Minimal public API for your hand-written C pacemaker implementation.
// Drop Pacemaker_C.c next to this header and implement these functions.
// Until then, these inline stubs keep the project compiling and do nothing.
#ifndef PACEMAKER_C_H
#define PACEMAKER_C_H


#ifdef __cplusplus
extern "C" {
#endif


// Call once at startup. Return 0 on success.
int PMc_init(void);


// Start a 1 ms system tick (e.g., alt_alarm_start(..., 1, ...)).
// Optional; you may ignore if you solely use PMc_run_for_elapsed_ms.
int PMc_start_1ms_alarm(void);


// Provide one-tick input pulses from atrium/ventricle sensors.
void PMc_set_senses(int AS_pulse, int VS_pulse);


// Advance the internal model by the given elapsed milliseconds.
// Typical implementation: consume N times a 1ms step function.
void PMc_run_for_elapsed_ms(int elapsed_ms);


// Retrieve and clear one-shot output pulses (AP/VP) produced since last call.
void PMc_poll_and_clear_pulses(int *AP_pulse, int *VP_pulse);


// ---------------- Default no-op stubs (compile without Pacemaker_C.c) ----------------
#ifndef PACEMAKER_C_AVAILABLE
static inline int PMc_init(void){ return 0; }
static inline int PMc_start_1ms_alarm(void){ return 0; }
static inline void PMc_set_senses(int a, int v){ (void)a; (void)v; }
static inline void PMc_run_for_elapsed_ms(int ms){ (void)ms; }
static inline void PMc_poll_and_clear_pulses(int *ap, int *vp){ if(ap)*ap=0; if(vp)*vp=0; }
#endif


#ifdef __cplusplus
}
#endif


#endif /* PACEMAKER_C_H */
