/*
 * main.c — COMPSYS 303 Pacemaker (SCCharts + C-mode) with button self‑test prints
 *
 * Modes (per assignment):
 *   SW0: 0 = SCCharts   | 1 = C implementation
 *   SW1: 0 = Buttons    | 1 = UART (virtual heart / PuTTY)
 *
 * I/O mapping:
 *   KEY1 -> AS (active‑low)   |  KEY0 -> VS (active‑low)
 *   LEDG1 <- AP               |  LEDG0 <- VP
 *   LEDR[1:0] mirror SW[1:0]
 *
 * Console: STDOUT=/dev/jtag_uart (Nios II Console)
 * Hardware UART: /dev/uart @ 115200 8‑N‑1, no flow control
 */

#include <stdio.h>
#include <string.h>
#include <system.h>
#include "Pacemaker.h"     // SCCharts API: reset(), tick(), TickData
#include "Pacemaker_C.h"   // C-mode API: PMc_*
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

#define HEARTBEAT_MS 1000

// ===== SCCharts globals =====
static TickData g_pm;

// ===== Shared HW state =====
static int      g_uart_fd   = -1;
static alt_u32  g_last_tick = 0;
static alt_u32  g_last_hb_ms= 0;
static alt_u32  g_prev_keys = 0xFFFFFFFF; // for button edge detection

// ===== C-mode globals =====
static PacemakerC g_c;          // C-mode state
static int        g_c_inited = 0;

// ===== Helpers =====
static inline alt_u32 ms_now(void){
  alt_u64 t = alt_timestamp();
  alt_u32 f = alt_timestamp_freq();
  return (alt_u32)((t * 1000ULL) / (f ? f : 1));
}

static void leds_show_mode(alt_u32 sw){
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_RED_BASE, (sw & 0x03));
}

static void leds_show_pace(int AP, int VP){
  alt_u32 g = 0;
  if (AP) g |= 0x02; // LEDG1
  if (VP) g |= 0x01; // LEDG0
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, g);
}

// Buttons path: active‑low, edge‑detect presses → one‑tick pulses + debug print
static void handle_buttons_inputs_with_debug_SCCharts(void){
  alt_u32 keys = IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);
  int as_pressed = ((g_prev_keys & 0x02) != 0) && ((keys & 0x02) == 0);
  int vs_pressed = ((g_prev_keys & 0x01) != 0) && ((keys & 0x01) == 0);
  if (as_pressed){ g_pm.AS = 1; printf("[BTN] AS pressed (KEY1) @ %ums\n", ms_now()); }
  if (vs_pressed){ g_pm.VS = 1; printf("[BTN] VS pressed (KEY0) @ %ums\n", ms_now()); }
  g_prev_keys = keys;
}

static void handle_uart_inputs_SCCharts(void){
  char ch; int n;
  while ((n = read(g_uart_fd, &ch, 1)) > 0){
    if (ch == 'A' || ch == 'a') g_pm.AS = 1;
    if (ch == 'V' || ch == 'v') g_pm.VS = 1;
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
  alt_u32 now = ms_now();
  if (now - g_last_hb_ms >= HEARTBEAT_MS){
    g_last_hb_ms = now;
    printf("[HB] t=%ums\n", now);
    fflush(stdout);
  }
}

// ===== Main =====
int main(void){
  // SCCharts init
  reset(&g_pm);

  // Timestamp for SCCharts deltaT
  if (alt_timestamp_start() < 0){ printf("[ERR] alt_timestamp_start failed\n"); }
  g_last_tick  = alt_timestamp();
  g_last_hb_ms = 0;
  g_prev_keys  = IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE); // seed edge detector

  // Banner
  printf("\n==== COMPSYS303 Pacemaker (SCCharts + C-mode) ====\n");
  printf("UART dev: %s (115200 8N1)\n", UART_NAME);
  printf("Timings: AVI=%d AEI=%d PVARP=%d VRP=%d LRI=%d URI=%d (ms)\n",
         AVI_VALUE, AEI_VALUE, PVARP_VALUE, VRP_VALUE, LRI_VALUE, URI_VALUE);
  printf("SW0: 0=SCCharts,1=C  |  SW1: 0=Buttons,1=UART\n\n");
  fflush(stdout);

  // Open hardware UART non‑blocking
  g_uart_fd = open(UART_NAME, O_RDWR | O_NONBLOCK);
  if (g_uart_fd < 0) {
    printf("[ERR] open(%s) failed — check SOPC name/pins/cable.\n", UART_NAME);
  } else {
    printf("[OK ] %s opened.\n", UART_NAME);
    uart_send_probe();
  }
  fflush(stdout);

  while (1){
    // Read switches and mirror on red LEDs
    alt_u32 sw = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);
    leds_show_mode(sw);

    // Mode decode
    int sccharts_mode = ((sw & 0x01) == 0); // SW0=0 -> SCCharts
    int uart_source   = ((sw & 0x02) != 0); // SW1=1 -> UART

    if (sccharts_mode){
      // ===== SCCharts path =====
      // Compute deltaT in milliseconds for SCCharts
      alt_u32 now_ticks = alt_timestamp();
      alt_u32 elapsed   = now_ticks - g_last_tick;
      g_last_tick       = now_ticks;
      g_pm.deltaT = (double)elapsed * 1000.0 / (double)alt_timestamp_freq();

      if (uart_source)  handle_uart_inputs_SCCharts();
      else              handle_buttons_inputs_with_debug_SCCharts();

      tick(&g_pm);
      leds_show_pace(g_pm.AP, g_pm.VP);
      uart_send_pace_bytes_if_enabled(uart_source, g_pm.AP, g_pm.VP);

      // clear one‑tick senses
      g_pm.AS = 0; g_pm.VS = 0;
    } else {
      // ===== C-mode path =====
      if (!g_c_inited){
        PMc_init(&g_c);
        PMc_set_led_pulse_ms(&g_c, 25); // human‑visible LEDs
        if (PMc_start_1ms_alarm() < 0) printf("[ERR] 1ms alarm start failed\n");
        g_c_inited = 1;
      }

      // Gather senses for this iteration (single‑ms pulses)
      int AS_raw = 0, VS_raw = 0;
      if (uart_source){
        char ch; int n;
        while ((n = read(g_uart_fd, &ch, 1)) > 0){
          if (ch == 'A' || ch == 'a') AS_raw = 1;
          if (ch == 'V' || ch == 'v') VS_raw = 1;
        }
      } else {
        alt_u32 keys = IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);
        int as_pressed = ((g_prev_keys & 0x02) != 0) && ((keys & 0x02) == 0);
        int vs_pressed = ((g_prev_keys & 0x01) != 0) && ((keys & 0x01) == 0);
        if (as_pressed){ AS_raw = 1; printf("[BTN] AS pressed (KEY1) @ %ums\n", ms_now()); }
        if (vs_pressed){ VS_raw = 1; printf("[BTN] VS pressed (KEY0) @ %ums\n", ms_now()); }
        g_prev_keys = keys;
      }

      // Feed senses and run for however many ms elapsed since last ISR poll
      PMc_set_senses(&g_c, AS_raw, VS_raw);
      PMc_run_for_elapsed_ms(&g_c);

      // LEDs from C-mode (stretched in module)
      alt_u32 g = 0;
      if (PMc_led_AP_on(&g_c)) g |= 0x02;
      if (PMc_led_VP_on(&g_c)) g |= 0x01;
      IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, g);

      // Push any AP/VP pulses over UART (if UART mode)
      int ap_any=0, vp_any=0;
      PMc_poll_and_clear_pulses(&g_c, &ap_any, &vp_any);
      if (uart_source && g_uart_fd >= 0){
        if (ap_any){ const char A='A'; (void)write(g_uart_fd,&A,1); }
        if (vp_any){ const char V='V'; (void)write(g_uart_fd,&V,1); }
      }
    }

    heartbeat_stdout();
  }

  return 0;
}
