#include <string.h>
#include <stdio.h>
#include <sys/alt_alarm.h>
#include <altera_avalon_pio_regs.h>
#include "Pacemaker_C.h"
#include <system.h>

/* Module-local 1ms ticker */
static volatile alt_u32 s_ms_ticks = 0;
static alt_alarm s_alarm;
static int s_banner_printed = 0;
/* NEW: running logical time (ms) for readable logs */
static alt_u32 s_now_ms = 0;

static alt_u32 PMc_alarm_cb(void* ctx) {
  (void)ctx;
  s_ms_ticks++;
  return 1; /* schedule next callback in 1 ms */
}

int PMc_start_1ms_alarm(void) {
  int r = alt_alarm_start(&s_alarm, 1, PMc_alarm_cb, NULL);
  if (!s_banner_printed) {
    s_banner_printed = 1;
    printf("[C-MODE] C pacemaker running (ISR 1ms tick active)\n");
    fflush(stdout);
  }
  return r;
}

static void dec_if_pos(int* t) { if (*t > 0) (*t)--; }

void PMc_init(PacemakerC* s) {
  memset(s, 0, sizeof(*s));
  s->led_pulse_ms = 25;   /* default visibility */
  s->LRI = LRI_VALUE;     /* start backup V timer so a VP will occur eventually */
}

void PMc_set_led_pulse_ms(PacemakerC* s, int ms) {
  s->led_pulse_ms = (ms > 0 ? ms : 1);
}

void PMc_set_senses(PacemakerC* s, int AS_raw, int VS_raw) {
  s->AS_raw = AS_raw;
  s->VS_raw = VS_raw;
  /* Optional sanity check: KEY2 press forces a print that confirms C-mode alive */
  alt_u32 keys = IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);
  if ((keys & 0x04) == 0) { /* KEY2 active-low */
    printf("[C-MODE] Sanity button (KEY2) pressed ~ C version confirmed running.\n");
    fflush(stdout);
  }
}

/* One 1-ms algorithm step — fixed DDD semantics */
static void PMc_tick_1ms(PacemakerC* s) {
  /* advance logical time for logging */
  s_now_ms++;

  /* Decrement timers and LED stretch */
  dec_if_pos(&s->AVI); dec_if_pos(&s->AEI); dec_if_pos(&s->PVARP);
  dec_if_pos(&s->VRP); dec_if_pos(&s->LRI); dec_if_pos(&s->URI);
  dec_if_pos(&s->AP_led_ms); dec_if_pos(&s->VP_led_ms);

  /* Gate senses by refractory (AR = in PVARP, VR = in VRP) */
  s->AS = (s->AS_raw && s->PVARP == 0);
  s->VS = (s->VS_raw && s->VRP   == 0);

  /* Default outputs this ms */
  s->AP = 0; s->VP = 0;

  /* -------- INTRINSIC EVENTS FIRST -------- */

  /* Ventricular sense: start post-V windows, reset A-tracking, stop A->V wait */
  if (s->VS) {
    s->VRP = VRP_VALUE;          /* post-V ventricular refractory */
    s->PVARP = PVARP_VALUE;      /* post-V atrial refractory (ONLY here) */
    s->AEI = AEI_VALUE;          /* V->A interval */
    s->LRI = LRI_VALUE;          /* backup V-V interval */
    s->URI = URI_VALUE;          /* upper rate gate */
    s->seen_AS_since_last_V = 0;
    s->AVI = 0;                  /* valid V cancels pending AVI */
    s->vp_waiting_for_URI = 0;   /* clear any postponed VP */
  }

  /* Atrial sense: stop AEI, (re)start AVI to wait for V, track that A happened */
  if (s->AS) {
    s->AEI = 0;                  /* stop V->A wait because A arrived */
    s->AVI = AVI_VALUE;          /* start A->V wait */
    s->seen_AS_since_last_V = 1;
  }

  /* -------- PACING DECISIONS -------- */

  /* Atrial pacing on AEI timeout ONLY if no AS since the last V */
  if (s->AEI == 0 && s->seen_AS_since_last_V == 0) {
    s->AP = 1; s->AP_fired = 1; s->AP_led_ms = s->led_pulse_ms;

    /* NEW: explain exactly why AP fired */
    printf("[AP] %lu ms — AEI timeout with no atrial sense since last V. "
           "(AEI=0, AVI=%d, PVARP=%d, VRP=%d, LRI=%d, URI=%d)\n",
           (unsigned long)s_now_ms, s->AVI, s->PVARP, s->VRP, s->LRI, s->URI);
    fflush(stdout);

    /* After an AP: do NOT start PVARP (that is post-V only) */
    s->AVI = AVI_VALUE;          /* now wait for V */
    s->seen_AS_since_last_V = 1; /* we just created an atrial event */
  }

  /* Ventricular pacing desire: from AVI timeout (when tracking A) or from LRI */
  {
    int want_VP = 0;
    if (s->AVI == 0 && s->seen_AS_since_last_V) want_VP = 1; /* A-tracked pair */
    if (s->LRI == 0)                            want_VP = 1; /* backup rate */

    if (want_VP) {
      if (s->URI == 0) {
        /* Deliver VP now */
        s->VP = 1; s->VP_fired = 1; s->VP_led_ms = s->led_pulse_ms;
        s->VRP = VRP_VALUE; s->PVARP = PVARP_VALUE;
        s->AEI = AEI_VALUE; s->LRI = LRI_VALUE; s->URI = URI_VALUE;
        s->AVI = 0; s->vp_waiting_for_URI = 0; s->seen_AS_since_last_V = 0;
      } else {
        /* URI still active → postpone VP until URI expires */
        s->vp_waiting_for_URI = 1;
      }
    }

    /* If a VP was postponed by URI, fire it as soon as URI expires */
    if (s->vp_waiting_for_URI && s->URI == 0) {
      s->VP = 1; s->VP_fired = 1; s->VP_led_ms = s->led_pulse_ms;
      s->VRP = VRP_VALUE; s->PVARP = PVARP_VALUE;
      s->AEI = AEI_VALUE; s->LRI = LRI_VALUE; s->URI = URI_VALUE;
      s->AVI = 0; s->vp_waiting_for_URI = 0; s->seen_AS_since_last_V = 0;
    }
  }

  /* Raw senses are 1-ms pulses; clear after consumption */
  s->AS_raw = 0; s->VS_raw = 0;
}


void PMc_run_for_elapsed_ms(PacemakerC* s) {
  while (s_ms_ticks) {
    PMc_tick_1ms(s);
    s_ms_ticks--;
  }
}

void PMc_poll_and_clear_pulses(PacemakerC* s, int* AP_any, int* VP_any) {
  if (AP_any) *AP_any = s->AP_fired;
  if (VP_any) *VP_any = s->VP_fired;
  s->AP_fired = 0;
  s->VP_fired = 0;
}

int PMc_led_AP_on(const PacemakerC* s) { return (s->AP_led_ms > 0); }
int PMc_led_VP_on(const PacemakerC* s) { return (s->VP_led_ms > 0); }
