#ifndef PACEMAKER_C_H
#define PACEMAKER_C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Public API (declarations only). Implementations live in Pacemaker_C.c */
int  PMc_init(void);
int  PMc_start_1ms_alarm(void);
void PMc_set_senses(int AS_pulse, int VS_pulse);
void PMc_run_for_elapsed_ms(int elapsed_ms);
void PMc_poll_and_clear_pulses(int *AP_pulse, int *VP_pulse);

/* -------------------------------------------------------------
   Optional header-only STUBS (disabled by default)
   Enable temporarily if you build without Pacemaker_C.c.
   ------------------------------------------------------------- */
#ifdef PM_C_HEADER_STUBS
static inline int  PMc_init(void){ return 0; }
static inline int  PMc_start_1ms_alarm(void){ return 0; }
static inline void PMc_set_senses(int a, int v){ (void)a; (void)v; }
static inline void PMc_run_for_elapsed_ms(int ms){ (void)ms; }
static inline void PMc_poll_and_clear_pulses(int *ap, int *vp){ if(ap)*ap=0; if(vp)*vp=0; }
#endif /* PM_C_HEADER_STUBS */

#ifdef __cplusplus
}
#endif

#endif /* PACEMAKER_C_H */
