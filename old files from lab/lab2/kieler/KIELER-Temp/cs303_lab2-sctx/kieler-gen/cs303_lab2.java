/*
 * Automatically generated Java code by
 * KIELER SCCharts - The Key to Efficient Modeling
 *
 * http://rtsys.informatik.uni-kiel.de/kieler
 */

public class cs303_lab2 {

  public boolean A;
  public boolean B;
  public boolean R;
  public boolean O;
  public int[] _taken_transitions;
  public boolean _g5;
  public boolean _g6;
  public boolean _g7;
  public boolean _g8;
  public boolean _g11;
  public boolean _g12;
  public boolean _g13;
  public boolean _g14;
  public boolean _g15;
  public boolean _g16;
  public boolean _g17;
  public boolean _g21;
  public boolean _GO;
  public boolean _cg7;
  public boolean _cg5;
  public boolean _cg13;
  public boolean _cg11;
  public boolean _cg15;
  public boolean _cg20;
  public boolean _g6_e1;
  public boolean _g12_e2;
  public boolean _g16_e1;
  public boolean _g21_e2;
  public boolean _TERM;

  public cs303_lab2() {
    _taken_transitions = new int[4];
  }
  public boolean _pg16;
  public boolean _pg21;
  public boolean _pg21_e2;

  public void reset() {
    _GO = true;
    _TERM = false;
    O = false;
    _taken_transitions[0] = 0;
    _taken_transitions[1] = 0;
    _taken_transitions[2] = 0;
    _taken_transitions[3] = 0;
    _pg16 = false;
    _pg21 = false;
    _pg21_e2 = false;
  }

  public void logic() {
    _g5 = _pg16;
    _g11 = _pg21;
    _g16_e1 = !(_g5 || _g11);
    _g6_e1 = !_g5;
    _g7 = _g5 && !R;
    _g8 = _g7 && A;
    if (_g8) {
      _taken_transitions[2] += 1;
    }
    _g5 = _g5 && R || _g8;
    _g8 = !_g11;
    _g13 = _g11 && !R;
    _g14 = _g13 && B;
    if (_g14) {
      _taken_transitions[3] += 1;
    }
    _g11 = _g11 && R || _g14;
    _g14 = (_g6_e1 || _g5) && (_g8 || _g11) && (_g5 || _g11);
    _g6_e1 = _g14 && !R;
    if (_g6_e1) {
      O = true;
      _taken_transitions[1] += 1;
    }
    _g12 = _g14 && R || _g6_e1;
    _g12_e2 = _pg21_e2;
    _g6 = !_g12_e2;
    _g15 = _g12_e2 && R;
    _g17 = (_g16_e1 || _g12) && (_g6 || _g15) && (_g12 || _g15);
    if (_g17) {
      _taken_transitions[0] += 1;
    }
    _g16_e1 = _GO || _g17;
    if (_g16_e1) {
      O = false;
    }
    _g16 = _g16_e1 || _g7 && !A;
    _g21 = _g16_e1 || _g13 && !B;
    _g21_e2 = _g16_e1 || _g12_e2 && !R;
  }

  public void tick() {
    logic();

    _pg16 = _g16;
    _pg21 = _g21;
    _pg21_e2 = _g21_e2;
    _GO = false;
  }
}
