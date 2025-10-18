#include "Pacemaker_C.h"

// ----- Core stubbed pacemaker API -----
void PMc_init(PacemakerC* s){
  if (!s) return;
  s->dbg_period_ms = 1000; // default 1s
  s->dbg_accum_ms  = 0;
  s->dbg_led7      = 0;
}

void PMc_set_led_pulse_ms(PacemakerC* s, int ms){ (void)s; (void)ms; }
int  PMc_start_1ms_alarm(void){ return 0; }
void PMc_set_senses(PacemakerC* s, int AS_raw, int VS_raw){ (void)s; (void)AS_raw; (void)VS_raw; }
void PMc_run_for_elapsed_ms(PacemakerC* s){ (void)s; }
void PMc_poll_and_clear_pulses(PacemakerC* s, int* AP_any, int* VP_any){ if (AP_any) *AP_any = 0; if (VP_any) *VP_any = 0; }
int  PMc_led_AP_on(const PacemakerC* s){ (void)s; return 0; }
int  PMc_led_VP_on(const PacemakerC* s){ (void)s; return 0; }

// ----- Debug blinker (LEDG7) -----
void PMc_debug_set_blink_period_ms(PacemakerC* s, int period_ms){
  if (!s) return;
  s->dbg_period_ms = period_ms;
  if (s->dbg_period_ms <= 0){
    s->dbg_period_ms = 0;
    s->dbg_accum_ms  = 0;
    s->dbg_led7      = 0;
  }
}

void PMc_debug_tick_elapsed_ms(PacemakerC* s, int elapsed_ms){
  if (!s) return;
  if (s->dbg_period_ms <= 0) return;
  if (elapsed_ms <= 0) return;

  s->dbg_accum_ms += elapsed_ms;
  while (s->dbg_accum_ms >= s->dbg_period_ms){
    s->dbg_accum_ms -= s->dbg_period_ms;
    s->dbg_led7 ^= 1; // toggle each full period
  }
}

int PMc_debug_led7(const PacemakerC* s){
  if (!s) return 0;
  return s->dbg_led7 ? 1 : 0;
}
