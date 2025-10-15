#ifndef CS303_LAB2_H
#define CS303_LAB2_H
/*
 * Automatically generated C code by
 * KIELER SCCharts - The Key to Efficient Modeling
 *
 * http://rtsys.informatik.uni-kiel.de/kieler
 */


typedef struct {
  char A;
  int _taken_transitions[1];
  char _g1;
  char _g2;
  char _GO;
  char _cg2;
  char _TERM;
  char _pg1;
} TickData;

void reset(TickData* d);
void logic(TickData* d);
void tick(TickData* d);

#endif /* !CS303_LAB2_H */
