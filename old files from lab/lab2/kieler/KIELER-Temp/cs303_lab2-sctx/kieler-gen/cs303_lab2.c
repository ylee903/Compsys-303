/*
 * Automatically generated C code by
 * KIELER SCCharts - The Key to Efficient Modeling
 *
 * http://rtsys.informatik.uni-kiel.de/kieler
 */

#include "cs303_lab2.h"

void logic(TickData* d) {
  d->_g2 = d->_pg1;
  d->_g1 = d->_GO || d->_g2 && !d->A;
  d->_TERM = d->_g2 && d->A;
  if (d->_TERM) {
    d->_taken_transitions[0] += 1;
  }
}

void reset(TickData* d) {
  d->_GO = 1;
  d->_TERM = 0;
  d->_taken_transitions[0] = 0;
  d->_pg1 = 0;
}

void tick(TickData* d) {
  logic(d);

  d->_pg1 = d->_g1;
  d->_GO = 0;
}
