/*
 * main.c — COMPSYS 303 Pacemaker (SCCharts + C-mode)
 *
 * LED freeze fix: switch LED timing from absolute timestamps to
 * countdown counters updated by elapsed ms → robust to timer wrap.
 */

#include <stdio.h>
#include <string.h>
#include <system.h>
#include "Pacemaker.h"
#include "Pacemaker_C.h"
#include "timing.h"
#include <altera_avalon_pio_regs.h>
#include <alt_types.h>
#include <sys/alt_irq.h>
#include <sys/alt_timestamp.h>
#include "sys/alt_alarm.h"
#include <fcntl.h>
#include <unistd.h>

#ifndef UART_NAME
  #define UART_NAME "/dev/uart"
#endif

#define HEARTBEAT_MS   1000
#define LED_STRETCH_MS 150   // visible duration for AP/VP paces (ms)
#define LED_INPUT_MS   60    // short input blip for manual senses (ms)

static TickData g_pm;
static int      g_uart_fd   = -1;
static alt_u32  g_last_tick = 0;
static alt_u32  g_last_hb_ms= 0;
static alt_u32  g_prev_keys = 0xFFFFFFFF;

static PacemakerC g_c;
static int        g_c_inited = 0;

// LED stretch state — now countdowns (ms), not absolute times
static int ap_led_ms = 0; // AP pace LED
static int vp_led_ms = 0; // VP pace LED
static int as_led_ms = 0; // manual AS input blip
static int vs_led_ms = 0; // manual VS input blip

static inline alt_u32 ms_now(void){
  alt_u64 t = alt_timestamp();
  alt_u32 f = alt_timestamp_freq();
  return (alt_u32)((t * 1000ULL) / (f ? f : 1));
}

static void leds_show_mode(alt_u32 sw){
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_RED_BASE, (sw & 0x03));
}

// Update LED GPIO based on countdowns (>0 → ON)
static inline void leds_apply_from_counters(void){
  alt_u32 g = 0;
  if (ap_led_ms > 0 || as_led_ms > 0) g |= 0x02; // LEDG1
  if (vp_led_ms > 0 || vs_led_ms > 0) g |= 0x01; // LEDG0
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, g);
}

// Decrement all LED counters by elapsed_ms (clamp at 0)
static inline void leds_decay(int elapsed_ms){
  if (elapsed_ms <= 0) return;
  if (ap_led_ms > 0) { ap_led_ms -= elapsed_ms; if (ap_led_ms < 0) ap_led_ms = 0; }
  if (vp_led_ms > 0) { vp_led_ms -= elapsed_ms; if (vp_led_ms < 0) vp_led_ms = 0; }
  if (as_led_ms > 0) { as_led_ms -= elapsed_ms; if (as_led_ms < 0) as_led_ms = 0; }
  if (vs_led_ms > 0) { vs_led_ms -= elapsed_ms; if (vs_led_ms < 0) vs_led_ms = 0; }
}

// Trigger only when button goes from unpressed (1) → pressed (0)
static void main_read_buttons_on_press(int *AS_btn, int *VS_btn) {
  *AS_btn = 0; *VS_btn = 0;
  alt_u32 keys = IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);
  if (((g_prev_keys & 0x02) != 0) && ((keys & 0x02) == 0)) {
    *AS_btn = 1; as_led_ms = LED_INPUT_MS; // blip input LED
    printf("[MAIN][BTN] AS PRESS @ %ums\n", ms_now());
  }
  if (((g_prev_keys & 0x01) != 0) && ((keys & 0x01) == 0)) {
    *VS_btn = 1; vs_led_ms = LED_INPUT_MS; // blip input LED
    printf("[MAIN][BTN] VS PRESS @ %ums\n", ms_now());
  }
  g_prev_keys = keys;
}

static void main_poll_uart_senses(int *AS_uart, int *VS_uart) {
  *AS_uart = 0; *VS_uart = 0;
  if (g_uart_fd < 0) return;
  char ch; int n;
  while ((n = read(g_uart_fd, &ch, 1)) > 0) {
    if (ch == 'A' || ch == 'a') *AS_uart = 1;
    if (ch == 'V' || ch == 'v') *VS_uart = 1;
  }
}

static void uart_send_probe(void){
  if (g_uart_fd >= 0) {
    const char *probe = "HELLO_FROM_BOARD\r\n";
    (void)write(g_uart_fd, probe, (int)strlen(probe));
  }
}

static void uart_send_pace_bytes_if_enabled(int uart_enabled, int AP, int VP){
  if (!uart_enabled || g_uart_fd < 0) return;
  if (AP) { const char A = 'A'; (void)write(g_uart_fd, &A, 1); }
  if (VP) { const char V = 'V'; (void)write(g_uart_fd, &V, 1); }
}

static void heartbeat_stdout(void){
  static alt_u32 hb_accum = 0; // use elapsed for HB too, to survive wraps
  alt_u32 now_ticks = alt_timestamp();
  alt_u32 elapsed   = now_ticks - g_last_tick; // used only for HB pace
  g_last_tick       = now_ticks;
  alt_u32 f = alt_timestamp_freq();
  alt_u32 elapsed_ms = (alt_u32)((elapsed * 1000ULL) / (f ? f : 1));
  hb_accum += elapsed_ms;
  if (hb_accum >= HEARTBEAT_MS){ hb_accum -= HEARTBEAT_MS; printf("[HB]\n"); fflush(stdout);}
}

int main(void){
  reset(&g_pm);

  if (alt_timestamp_start() < 0){ printf("[ERR] alt_timestamp_start failed\n"); }
  g_last_tick  = alt_timestamp();
  g_prev_keys  = IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);

  printf("\n==== COMPSYS303 Pacemaker (SCCharts + C-mode) ====\n");
  printf("UART dev: %s (115200 8N1)\n", UART_NAME);
  printf("Timings: AVI=%d AEI=%d PVARP=%d VRP=%d LRI=%d URI=%d (ms)\n",
         AVI_VALUE, AEI_VALUE, PVARP_VALUE, VRP_VALUE, LRI_VALUE, URI_VALUE);
  printf("SW0: 0=SCCharts,1=C  |  SW1: 0=Buttons,1=UART\n\n");
  fflush(stdout);

  g_uart_fd = open(UART_NAME, O_RDWR | O_NONBLOCK);
  if (g_uart_fd < 0) {
    printf("[ERR] open(%s) failed — check pins/cable.\n", UART_NAME);
  } else {
    printf("[OK ] %s opened.\n", UART_NAME);
    uart_send_probe();
  }
  fflush(stdout);

  // Separate timebase for application logic (so HB change above doesn't affect deltaT)
  alt_u32 last_ticks_logic = alt_timestamp();

  while (1){
    // Compute elapsed_ms once per loop; use for both modes and LED decay
    alt_u32 now_ticks = alt_timestamp();
    alt_u32 elapsed_ticks = now_ticks - last_ticks_logic;
    last_ticks_logic = now_ticks;
    alt_u32 f = alt_timestamp_freq();
    int elapsed_ms = (int)((elapsed_ticks * 1000ULL) / (f ? f : 1));
    if (elapsed_ms <= 0) elapsed_ms = 1; // be conservative

    alt_u32 sw = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);
    leds_show_mode(sw);

    int sccharts_mode = ((sw & 0x01) == 0);
    int uart_source   = ((sw & 0x02) != 0);

    if (sccharts_mode){
      // SCCharts deltaT in ms from elapsed_ms
      g_pm.deltaT = (double)elapsed_ms;

      int AS_btn=0, VS_btn=0, AS_uart=0, VS_uart=0;
      main_read_buttons_on_press(&AS_btn, &VS_btn);
      if (uart_source) main_poll_uart_senses(&AS_uart, &VS_uart);
      g_pm.AS = (AS_btn || AS_uart);
      g_pm.VS = (VS_btn || VS_uart);

      tick(&g_pm);

      // On paces, start pace LED counters
      if (g_pm.AP) ap_led_ms = LED_STRETCH_MS;
      if (g_pm.VP) vp_led_ms = LED_STRETCH_MS;

      uart_send_pace_bytes_if_enabled(uart_source, g_pm.AP, g_pm.VP);
      g_pm.AS = 0; g_pm.VS = 0;

    } else {
      if (!g_c_inited){
        PMc_init(&g_c);
        PMc_set_led_pulse_ms(&g_c, 25);
        if (PMc_start_1ms_alarm() < 0) printf("[ERR] 1ms alarm start failed\n");
        g_c_inited = 1;
      }

      int AS_btn=0, VS_btn=0, AS_uart=0, VS_uart=0;
      main_read_buttons_on_press(&AS_btn, &VS_btn);
      if (uart_source) main_poll_uart_senses(&AS_uart, &VS_uart);
      int AS_raw = (AS_btn || AS_uart);
      int VS_raw = (VS_btn || VS_uart);

      PMc_set_senses(&g_c, AS_raw, VS_raw);
      PMc_run_for_elapsed_ms(&g_c);

      int ap_any=0, vp_any=0;
      PMc_poll_and_clear_pulses(&g_c, &ap_any, &vp_any);

      if (ap_any) ap_led_ms = LED_STRETCH_MS;
      if (vp_any) vp_led_ms = LED_STRETCH_MS;

      if (uart_source && g_uart_fd >= 0){
        if (ap_any){ const char A='A'; (void)write(g_uart_fd,&A,1); }
        if (vp_any){ const char V='V'; (void)write(g_uart_fd,&V,1); }
      }
    }

    // Decay and apply LEDs based on counters
    leds_decay(elapsed_ms);
    leds_apply_from_counters();

    // Heartbeat based on decayed timebase
    static int hb_accum_ms = 0;
    hb_accum_ms += elapsed_ms;
    if (hb_accum_ms >= HEARTBEAT_MS){ hb_accum_ms -= HEARTBEAT_MS; printf("[HB]\n"); fflush(stdout);}
  }

  return 0;
}
