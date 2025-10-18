#include <system.h>
#include <sys/alt_irq.h>
#include <sys/alt_alarm.h>
#include <altera_avalon_pio_regs.h>

#include "timing.h"       /* AVI_VALUE, AEI_VALUE, ... */
#include "Pacemaker_C.h"  /* Minimal API */

/* ==================== Local flags & timers ==================== */
#define LRI_MASK  (1u << 0)
#define URI_MASK  (1u << 1)
#define AVI_MASK  (1u << 2)
#define AEI_MASK  (1u << 3)
#define PVARP_MASK (1u << 4)
#define VRP_MASK   (1u << 5)

/* Nios II 1-shot/periodic timer objects */
static alt_alarm LRIalarm, URIalarm, AVIalarm, AEIalarm, PVARPalarm, VRPalarm;

/* One-tick inputs/outputs (consumed in execC) */
static volatile char VS, VP, AS, AP;
/* Mode (BUTTON or UART). UART path is stubbed out for this minimal build. */
static volatile Mode currentMode = BUTTON;

/* Bitfield tracking which alarms are running */
static volatile unsigned char pacemakerFlags = 0x00;

/* ==================== UART stubs (no uart.h needed) ==================== */
static inline char getData(void) { return 0; }           /* no incoming bytes */
static inline void sendData(char c) { (void)c; }         /* no-op */
static volatile int rxIndex = 0, receiveFlag = 0;        /* satisfy old code */

/* ==================== Helpers ==================== */
static void startAlarm(alt_alarm *alarm);
static void stopAlarm(alt_alarm *alarm);

/* LED visibility stretch (simple 0..N ms counters) */
static unsigned char ledTimeVP = 0, ledTimeAP = 0;
static void drive_leds(unsigned char ledAPms, unsigned char ledVPms,
                       unsigned char AS_now, unsigned char VS_now) {
  /* Green LEDs: G0=VP, G1=AP (hold ~10ms so humans can see) */
  unsigned int g = 0;
  if (ledVPms < 10) g |= 0x01;
  if (ledAPms < 10) g |= 0x02;
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, g);

  /* Red LEDs: show sensed edges this tick (R0=VS, R1=AS) */
  unsigned int r = 0;
  if (VS_now) r |= 0x01;
  if (AS_now) r |= 0x02;
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_RED_BASE, r);
}

/* ==================== Public API ==================== */
void initC(void) {
  /* Stop everything and clear flags */
  alt_alarm_stop(&LRIalarm);
  alt_alarm_stop(&URIalarm);
  alt_alarm_stop(&AVIalarm);
  alt_alarm_stop(&AEIalarm);
  alt_alarm_stop(&PVARPalarm);
  alt_alarm_stop(&VRPalarm);

  pacemakerFlags = 0;
  VS = VP = AS = AP = 0;
  ledTimeVP = ledTimeAP = 0;

  /* Ensure LEDs start off */
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0);
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_RED_BASE, 0);
}

void execC(double dt, Mode mode, int *button) {
  (void)dt;                       /* timers are handled by alt_alarm */
  currentMode = mode;

  /* --- Consume inputs --- */
  if (currentMode == BUTTON) {
    /* button bit0=VS, bit1=AS (edge pulses provided by main) */
    VS = (*button & (1 << 0)) ? 1 : 0;
    AS = (*button & (1 << 1)) ? 1 : 0;
    *button = 0; /* consume pulses */
  } else {
    /* UART mode (stub: no incoming data unless you add a uart driver) */
    char ch = getData();
    if (ch) {
      VS = (ch == 'V' || ch == 'v');
      AS = (ch == 'A' || ch == 'a');
      rxIndex = 0; receiveFlag = 0;
    }
  }

  /* Stretch LED visibility counters */
  if (ledTimeVP < 255) ledTimeVP++;
  if (ledTimeAP < 255) ledTimeAP++;

  /********************************
   *            LOGIC
   ********************************/

  /* ========= ATRIAL EVENT (AS or AP) ========= */
  if (AS || AP) {
    /* If AEI is running and not in PVARP, stop AEI (author intent kept) */
    if ((pacemakerFlags & AEI_MASK) && !(pacemakerFlags & PVARP_MASK)) {
      stopAlarm(&AEIalarm);
    }
    /* If AVI not running and not in PVARP, start AVI (A->V window) */
    if (!(pacemakerFlags & AVI_MASK) && !(pacemakerFlags & PVARP_MASK)) {
      startAlarm(&AVIalarm);
    }
    AP = 0;                /* consume one-tick AP */
    ledTimeAP = 0;         /* refresh AP LED hold */
  }

  /* ========= VENTRICULAR EVENT (VS or VP) ========= */
  if (VS || VP) {
    /* If AVI running and not in VRP, cancel AVI (V arrived naturally) */
    if ((pacemakerFlags & AVI_MASK) && !(pacemakerFlags & VRP_MASK)) {
      stopAlarm(&AVIalarm);
    }
    /* Start PVARP (block atrial sensing) unless VRP says ignore */
    if (!(pacemakerFlags & PVARP_MASK) && !(pacemakerFlags & VRP_MASK)) {
      startAlarm(&PVARPalarm);
    }
    /* Start AEI (V->A watchdog) unless VRP says ignore */
    if (!(pacemakerFlags & AEI_MASK) && !(pacemakerFlags & VRP_MASK)) {
      startAlarm(&AEIalarm);
    }
    /* Start VRP + URI + LRI (ventricular slate) unless already in VRP */
    if (!(pacemakerFlags & VRP_MASK)) {
      startAlarm(&VRPalarm);
      startAlarm(&URIalarm);
      startAlarm(&LRIalarm);
    }
    VP = 0;                /* consume one-tick VP */
    ledTimeVP = 0;         /* refresh VP LED hold */
  }

  /* Drive LEDs each call (green stretched, red show current senses) */
  drive_leds(ledTimeAP, ledTimeVP, AS, VS);

  /* One-tick senses are consumed inside this call */
  VS = 0; AS = 0;
}

/* ==================== Alarm helpers ==================== */
static alt_u32 isrLRI(void *ctx);
static alt_u32 isrURI(void *ctx);
static alt_u32 isrAVI(void *ctx);
static alt_u32 isrAEI(void *ctx);
static alt_u32 isrVRP(void *ctx);
static alt_u32 isrPVARP(void *ctx);

static void startAlarm(alt_alarm *alarm) {
  alt_alarm_stop(alarm);

  if (alarm == &AVIalarm) {
    pacemakerFlags |= AVI_MASK;
    alt_alarm_start(alarm, AVI_VALUE, isrAVI, NULL);
  } else if (alarm == &AEIalarm) {
    pacemakerFlags |= AEI_MASK;
    alt_alarm_start(alarm, AEI_VALUE, isrAEI, NULL);
  } else if (alarm == &URIalarm) {
    pacemakerFlags |= URI_MASK;
    alt_alarm_start(alarm, URI_VALUE, isrURI, NULL);
  } else if (alarm == &LRIalarm) {
    pacemakerFlags |= LRI_MASK;
    alt_alarm_start(alarm, LRI_VALUE, isrLRI, NULL);
  } else if (alarm == &PVARPalarm) {
    pacemakerFlags |= PVARP_MASK;
    alt_alarm_start(alarm, PVARP_VALUE, isrPVARP, NULL);
  } else if (alarm == &VRPalarm) {
    pacemakerFlags |= VRP_MASK;
    alt_alarm_start(alarm, VRP_VALUE, isrVRP, NULL);
  }
}

static void stopAlarm(alt_alarm *alarm) {
  alt_alarm_stop(alarm);

  if (alarm == &AVIalarm)      pacemakerFlags &= ~AVI_MASK;
  else if (alarm == &AEIalarm) pacemakerFlags &= ~AEI_MASK;
  else if (alarm == &URIalarm) pacemakerFlags &= ~URI_MASK;
  else if (alarm == &LRIalarm) pacemakerFlags &= ~LRI_MASK;
  else if (alarm == &PVARPalarm) pacemakerFlags &= ~PVARP_MASK;
  else if (alarm == &VRPalarm)   pacemakerFlags &= ~VRP_MASK;
}

/******************
 *** TIMER ISRs ***
 ******************/
static alt_u32 isrLRI(void *ctx) {
  (void)ctx;
  /* Backup VP */
  VP = 1;
  pacemakerFlags &= ~LRI_MASK;
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, IORD_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE) | 0x01);
  sendData('V');  /* no-op in this build */
  alt_alarm_stop(&LRIalarm);
  return 0;       /* one-shot */
}

static alt_u32 isrURI(void *ctx) {
  (void)ctx;
  pacemakerFlags &= ~URI_MASK;
  alt_alarm_stop(&URIalarm);
  return 0;
}

static alt_u32 isrPVARP(void *ctx) {
  (void)ctx;
  pacemakerFlags &= ~PVARP_MASK;
  alt_alarm_stop(&PVARPalarm);
  return 0;
}

static alt_u32 isrAVI(void *ctx) {
  (void)ctx;
  /* URI has priority: if still in URI, extend by returning 1 tick until URI ends */
  if (pacemakerFlags & URI_MASK) {
    return 1;   /* re-arm in 1 ms until URI ends */
  }
  pacemakerFlags &= ~AVI_MASK;
  /* VP due to AVI timeout */
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, IORD_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE) | 0x01);
  VP = 1;
  sendData('V'); /* no-op in this build */
  alt_alarm_stop(&AVIalarm);
  return 0;
}

static alt_u32 isrVRP(void *ctx) {
  (void)ctx;
  pacemakerFlags &= ~VRP_MASK;
  alt_alarm_stop(&VRPalarm);
  return 0;
}

static alt_u32 isrAEI(void *ctx) {
  (void)ctx;
  pacemakerFlags &= ~AEI_MASK;
  /* AP due to AEI timeout */
  AP = 1;
  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, IORD_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE) | 0x02);
  sendData('A'); /* no-op in this build */
  alt_alarm_stop(&AEIalarm);
  return 0;
}
