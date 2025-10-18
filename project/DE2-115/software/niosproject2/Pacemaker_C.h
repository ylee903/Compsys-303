#ifndef PACEMAKER_C_H
#define PACEMAKER_C_H

/* Minimal, self-contained C-mode API (no external headers needed) */
typedef enum { BUTTON = 0, UART = 1 } Mode;

/* Initialise all timers/flags (safe to call more than once). */
void initC(void);

/* Run one step of the C-mode engine.
   - dt: elapsed milliseconds (use 1 if you’re looping at ~1 kHz or don’t care)
   - mode: BUTTON or UART (UART is stubbed to no-op in this minimal build)
   - button: bit0 = VS, bit1 = AS (edge/pulse). Function will consume & clear it. */
void execC(double dt, Mode mode, int *button);

#endif /* PACEMAKER_C_H */
