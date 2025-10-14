/*
 * Automatically generated C code by
 * KIELER SCCharts - The Key to Efficient Modeling
 *
 * http://rtsys.informatik.uni-kiel.de/kieler
 */

#include "timing.h"

#include "Pacemaker.h"

void logic(TickData* d) {
  d->_g16 = d->_pg8;
  d->_g10 = d->_pg29;
  d->_g13_e1 = !(d->_g16 || d->_g10);
  d->_g20 = d->_pg17;
  d->_g21 = d->_g20 && d->VS;
  if (d->_g21) {
    d->_Pacemaker_local__Atrig = 1;
  }
  d->_g181 = d->_pg151;
  if (d->_g181) {
    d->_region0_VRefrac_t += d->deltaT;
  }
  d->_g23 = d->_g20 && !d->VS;
  d->_cg23 = d->_region0_VRefrac_t >= d->T_VRP;
  d->_g24 = d->_g23 && d->_cg23;
  if (d->_g24) {
    d->_Pacemaker_local__Atrig1 = 1;
  }
  d->sleepT = 1000.0;
  d->_g16 = d->_g10 || d->_g16;
  d->_cg11 = d->_region0_VRefrac_t < d->T_VRP;
  d->_g10 = d->_g16 && d->_cg11;
  if (d->_g10) {
    d->sleepT = (d->sleepT < (d->T_VRP - d->_region0_VRefrac_t)) ? d->sleepT : (d->T_VRP - d->_region0_VRefrac_t);
  }
  d->_cg12 = d->_Pacemaker_local__Atrig || d->_Pacemaker_local__Atrig1;
  d->_g11 = d->_g16 && !d->_cg11;
  d->_cg14 = d->_Pacemaker_local__Atrig || d->_Pacemaker_local__Atrig1;
  d->_g13 = d->_g10 && d->_cg12 || d->_g11 && d->_cg14;
  d->_g20 = !d->_g20;
  d->_g24 = d->_g21 || d->_g24;
  d->_g21 = (d->_g13_e1 || d->_g13) && (d->_g20 || d->_g24) && (d->_g13 || d->_g24);
  d->_cg25 = d->_Pacemaker_local__Atrig;
  d->_g22 = d->_g21 && !d->_cg25;
  if (d->_g22) {
    d->_taken_transitions[3] += 1;
  }
  d->_g13 = d->_pg13_e1;
  d->_g13_e1 = d->_GO || d->_g13;
  if (d->_g13_e1) {
    d->_pre_VP = d->_reg_VP;
  }
  d->_g22_e2 = d->_pg27;
  d->_g217 = d->_g22_e2 && !d->VS;
  d->_cg28 = d->_pre_VP;
  d->_g27 = d->_GO || d->_g22 || d->_g217 && !d->_cg28;
  d->AP = 0;
  d->VP = 0;
  d->A_EVT = 0;
  d->V_EVT = 0;
  d->A_REFR = 0;
  d->V_REFR = 0;
  d->_g4 = d->_g22_e2 && d->VS;
  if (d->_g4) {
    d->V_EVT |= 1;
    d->_region0_VRefrac_t = 0;
    d->_taken_transitions[0] += 1;
  }
  d->_g25 = d->_g21 && d->_cg25;
  if (d->_g25) {
    d->V_REFR |= 1;
    d->_taken_transitions[2] += 1;
  }
  d->_g28 = d->_g217 && d->_cg28;
  if (d->_g28) {
    d->V_EVT |= 1;
    d->_region0_VRefrac_t = 0;
    d->_taken_transitions[1] += 1;
  }
  d->_g26 = d->_g4 || d->_g25 || d->_g28;
  if (d->_g26) {
    d->_Pacemaker_local__Atrig = 0;
    d->_Pacemaker_local__Atrig1 = 0;
  }
  d->_cg7 = d->_region0_VRefrac_t < d->T_VRP;
  d->_g5 = d->_g26 && d->_cg7;
  if (d->_g5) {
    d->sleepT = (d->sleepT < (d->T_VRP - d->_region0_VRefrac_t)) ? d->sleepT : (d->T_VRP - d->_region0_VRefrac_t);
  }
  d->_cg8 = d->_Pacemaker_local__Atrig || d->_Pacemaker_local__Atrig1;
  d->_g29 = d->_g5 && !d->_cg8 || d->_g10 && !d->_cg12;
  d->_g12 = d->_g26 && !d->_cg7;
  d->_cg17 = d->_Pacemaker_local__Atrig || d->_Pacemaker_local__Atrig1;
  d->_g8 = d->_g11 && !d->_cg14 || d->_g12 && !d->_cg17;
  d->_g17 = d->_g26 || d->_g23 && !d->_cg23;
  d->_g14 = d->_pg37;
  d->_g23 = d->_pg34;
  d->_g6 = !(d->_g14 || d->_g23);
  d->_g49 = d->_pg41;
  d->_g50 = d->_g49 && d->AS;
  if (d->_g50) {
    d->_Pacemaker_local__Atrig2 = 1;
  }
  d->_g185 = d->_pg154;
  if (d->_g185) {
    d->_region1_ARefrac_t += d->deltaT;
  }
  d->_g52 = d->_g49 && !d->AS;
  d->_cg52 = d->_region1_ARefrac_t >= d->T_PVAR;
  d->_g53 = d->_g52 && d->_cg52;
  if (d->_g53) {
    d->_Pacemaker_local__Atrig3 = 1;
  }
  d->_g45 = d->_g23 || d->_g14;
  d->_cg40 = d->_region1_ARefrac_t < d->T_PVAR;
  d->_g39 = d->_g45 && d->_cg40;
  if (d->_g39) {
    d->sleepT = (d->sleepT < (d->T_PVAR - d->_region1_ARefrac_t)) ? d->sleepT : (d->T_PVAR - d->_region1_ARefrac_t);
  }
  d->_cg41 = d->_Pacemaker_local__Atrig2 || d->_Pacemaker_local__Atrig3;
  d->_g40 = d->_g45 && !d->_cg40;
  d->_cg43 = d->_Pacemaker_local__Atrig2 || d->_Pacemaker_local__Atrig3;
  d->_g42 = d->_g39 && d->_cg41 || d->_g40 && d->_cg43;
  d->_g49 = !d->_g49;
  d->_g53 = d->_g50 || d->_g53;
  d->_g50 = (d->_g6 || d->_g42) && (d->_g49 || d->_g53) && (d->_g42 || d->_g53);
  d->_cg54 = d->_Pacemaker_local__Atrig2;
  d->_g51 = d->_g50 && !d->_cg54;
  if (d->_g51) {
    d->_taken_transitions[9] += 1;
  }
  d->_g42 = d->_pg52;
  if (d->_g42) {
    d->A_EVT |= 1;
    d->_taken_transitions[10] += 1;
  }
  d->_g51_e2 = d->_pg42_e1;
  d->_g42_e1 = d->_GO || d->_g51_e2;
  if (d->_g42_e1) {
    d->_pre_AP = d->_reg_AP;
  }
  d->_g213 = d->_pg62;
  d->_g57 = d->_g213 && !d->VS;
  d->_cg57 = d->_pre_VP;
  d->_g59 = d->_g57 && !d->_cg57;
  d->_g63 = d->_g59 && !d->AS;
  d->_cg63 = d->_pre_AP;
  d->_g62 = d->_GO || d->_g51 || d->_g42 || d->_g63 && !d->_cg63;
  d->_g56 = d->_g213 && d->VS;
  if (d->_g56) {
    d->_region1_ARefrac_t = 0;
    d->_taken_transitions[4] += 1;
  }
  d->_g33 = d->_g50 && d->_cg54;
  if (d->_g33) {
    d->A_REFR |= 1;
    d->_taken_transitions[8] += 1;
  }
  d->_g54 = d->_g57 && d->_cg57;
  if (d->_g54) {
    d->_region1_ARefrac_t = 0;
    d->_taken_transitions[5] += 1;
  }
  d->_g57 = d->_g56 || d->_g33 || d->_g54;
  if (d->_g57) {
    d->_Pacemaker_local__Atrig2 = 0;
    d->_Pacemaker_local__Atrig3 = 0;
  }
  d->_cg36 = d->_region1_ARefrac_t < d->T_PVAR;
  d->_g55 = d->_g57 && d->_cg36;
  if (d->_g55) {
    d->sleepT = (d->sleepT < (d->T_PVAR - d->_region1_ARefrac_t)) ? d->sleepT : (d->T_PVAR - d->_region1_ARefrac_t);
  }
  d->_cg37 = d->_Pacemaker_local__Atrig2 || d->_Pacemaker_local__Atrig3;
  d->_g34 = d->_g55 && !d->_cg37 || d->_g39 && !d->_cg41;
  d->_g58 = d->_g57 && !d->_cg36;
  d->_cg46 = d->_Pacemaker_local__Atrig2 || d->_Pacemaker_local__Atrig3;
  d->_g37 = d->_g40 && !d->_cg43 || d->_g58 && !d->_cg46;
  d->_g41 = d->_g57 || d->_g52 && !d->_cg52;
  d->_g46 = d->_g59 && d->AS;
  if (d->_g46) {
    d->_taken_transitions[6] += 1;
  }
  d->_g43 = d->_g63 && d->_cg63;
  if (d->_g43) {
    d->_taken_transitions[7] += 1;
  }
  d->_g52 = d->_g46 || d->_g43;
  d->_g35 = d->_pg72;
  d->_cg81 = d->A_EVT;
  d->_g59 = d->_g35 && d->_cg81;
  if (d->_g59) {
    d->_Pacemaker_local__Atrig4 = 1;
  }
  d->_g63 = d->_pg158;
  if (d->_g63) {
    d->_region2_AEI_t += d->deltaT;
  }
  d->_g64 = d->_g35 && !d->_cg81;
  d->_cg84 = d->_region2_AEI_t >= d->T_AEI;
  d->_g60 = d->_g64 && d->_cg84;
  if (d->_g60) {
    d->_Pacemaker_local__Atrig5 = 1;
  }
  d->_g81 = d->_pg83_e2;
  d->_cg68 = d->V_EVT;
  d->_g69 = d->_g81 && d->_cg68;
  if (d->_g69) {
    d->_region2_AEI_t = 0;
    d->_taken_transitions[11] += 1;
    d->_Pacemaker_local__Atrig4 = 0;
    d->_Pacemaker_local__Atrig5 = 0;
  }
  d->_g75 = d->_pg74;
  d->_g78 = d->_pg77;
  d->_g78 = d->_g69 || d->_g75 || d->_g78;
  d->_cg71 = d->_region2_AEI_t < d->T_AEI;
  d->_g75 = d->_g78 && !d->_cg71;
  d->_cg76 = d->_Pacemaker_local__Atrig4 || d->_Pacemaker_local__Atrig5;
  d->_g77 = d->_g75 && !d->_cg76;
  d->_g71 = d->_g78 && d->_cg71;
  if (d->_g71) {
    d->sleepT = (d->sleepT < (d->T_AEI - d->_region2_AEI_t)) ? d->sleepT : (d->T_AEI - d->_region2_AEI_t);
  }
  d->_cg72 = d->_Pacemaker_local__Atrig4 || d->_Pacemaker_local__Atrig5;
  d->_g74 = d->_g71 && !d->_cg72;
  d->_g73_e1 = !(d->_g77 || d->_g74);
  d->_g76 = d->_g71 && d->_cg72 || d->_g75 && d->_cg76;
  d->_g72 = d->_g69 || d->_g64 && !d->_cg84;
  d->_g84 = !d->_g72;
  d->_g69 = d->_g59 || d->_g60;
  d->_g85 = (d->_g73_e1 || d->_g76) && (d->_g84 || d->_g69) && (d->_g76 || d->_g69);
  d->_cg86 = d->_Pacemaker_local__Atrig4;
  d->_g82 = d->_g85 && d->_cg86;
  if (d->_g82) {
    d->_taken_transitions[12] += 1;
  }
  d->_g73 = d->_g85 && !d->_cg86;
  if (d->_g73) {
    d->AP |= 1;
    d->_taken_transitions[13] += 1;
  }
  d->_g83_e2 = d->_GO || d->_g82 || d->_g73 || d->_g81 && !d->_cg68;
  d->_g73_e1 = d->_pg100;
  d->_cg105 = d->V_EVT;
  d->_g83 = d->_g73_e1 && d->_cg105;
  if (d->_g83) {
    d->_Pacemaker_local__Atrig6 = 1;
  }
  d->_g86 = d->_pg163;
  if (d->_g86) {
    d->_region3_AVI_t += d->deltaT;
  }
  d->_g88 = d->_g73_e1 && !d->_cg105;
  d->_cg108 = d->_region3_AVI_t >= d->T_AVI;
  d->_g68 = d->_g88 && d->_cg108;
  if (d->_g68) {
    d->_Pacemaker_local__Atrig7 = 1;
  }
  d->_g87 = d->_pg159;
  d->_cg92 = d->A_EVT;
  d->_g105 = d->_g87 && d->_cg92;
  if (d->_g105) {
    d->_region3_AVI_t = 0;
    d->_taken_transitions[14] += 1;
    d->_Pacemaker_local__Atrig6 = 0;
    d->_Pacemaker_local__Atrig7 = 0;
  }
  d->_g99 = d->_pg98;
  d->_g102 = d->_pg101;
  d->_g99 = d->_g105 || d->_g99 || d->_g102;
  d->_cg95 = d->_region3_AVI_t < d->T_AVI;
  d->_g102 = d->_g99 && !d->_cg95;
  d->_cg100 = d->_Pacemaker_local__Atrig6 || d->_Pacemaker_local__Atrig7;
  d->_g101 = d->_g102 && !d->_cg100;
  d->_g95 = d->_g99 && d->_cg95;
  if (d->_g95) {
    d->sleepT = (d->sleepT < (d->T_AVI - d->_region3_AVI_t)) ? d->sleepT : (d->T_AVI - d->_region3_AVI_t);
  }
  d->_cg96 = d->_Pacemaker_local__Atrig6 || d->_Pacemaker_local__Atrig7;
  d->_g98 = d->_g95 && !d->_cg96;
  d->_g97_e1 = !(d->_g101 || d->_g98);
  d->_g96 = d->_g95 && d->_cg96 || d->_g102 && d->_cg100;
  d->_g100 = d->_g105 || d->_g88 && !d->_cg108;
  d->_g93 = !d->_g100;
  d->_g108 = d->_g83 || d->_g68;
  d->_g109 = (d->_g97_e1 || d->_g96) && (d->_g93 || d->_g108) && (d->_g96 || d->_g108);
  d->_cg110 = d->_Pacemaker_local__Atrig6;
  d->_g106 = d->_g109 && d->_cg110;
  if (d->_g106) {
    d->_taken_transitions[15] += 1;
  }
  d->_g107_e2 = d->_pg111;
  d->_cg114 = d->V_EVT;
  d->_g97 = d->_g107_e2 && d->_cg114;
  if (d->_g97) {
    d->_taken_transitions[17] += 1;
  }
  d->_g107 = d->_pg137;
  d->_cg150 = d->V_EVT;
  d->_g97_e1 = d->_g107 && d->_cg150;
  if (d->_g97_e1) {
    d->uri_active = 1;
  }
  d->_g162 = d->_pg171;
  d->_g156 = d->_pg150;
  d->_g159_e1 = !(d->_g162 || d->_g156);
  d->_g166 = d->_pg172;
  d->_cg166 = d->V_EVT;
  d->_g167 = d->_g166 && d->_cg166;
  if (d->_g167) {
    d->_Pacemaker_local__Atrig10 = 1;
  }
  d->_g201 = d->_pg152;
  if (d->_g201) {
    d->_region5_URI_t += d->deltaT;
  }
  d->_g169 = d->_g166 && !d->_cg166;
  d->_cg169 = d->_region5_URI_t >= d->T_URI;
  d->_g170 = d->_g169 && d->_cg169;
  if (d->_g170) {
    d->_Pacemaker_local__Atrig11 = 1;
  }
  d->_g156 = d->_g156 || d->_g162;
  d->_cg157 = d->_region5_URI_t < d->T_URI;
  d->_g162 = d->_g156 && d->_cg157;
  if (d->_g162) {
    d->sleepT = (d->sleepT < (d->T_URI - d->_region5_URI_t)) ? d->sleepT : (d->T_URI - d->_region5_URI_t);
  }
  d->_cg158 = d->_Pacemaker_local__Atrig10 || d->_Pacemaker_local__Atrig11;
  d->_g157 = d->_g156 && !d->_cg157;
  d->_cg160 = d->_Pacemaker_local__Atrig10 || d->_Pacemaker_local__Atrig11;
  d->_g159 = d->_g162 && d->_cg158 || d->_g157 && d->_cg160;
  d->_g166 = !d->_g166;
  d->_g170 = d->_g167 || d->_g170;
  d->_g167 = (d->_g159_e1 || d->_g159) && (d->_g166 || d->_g170) && (d->_g159 || d->_g170);
  d->_cg171 = d->_Pacemaker_local__Atrig10;
  d->_g168 = d->_g167 && !d->_cg171;
  if (d->_g168) {
    d->uri_active = 0;
    d->_taken_transitions[25] += 1;
  }
  d->_g168_e2 = d->_g107_e2 && !d->_cg114;
  d->_cg116 = d->uri_active == 0;
  d->_g159_e1 = d->_g168_e2 && d->_cg116;
  if (d->_g159_e1) {
    d->VP |= 1;
    d->_taken_transitions[18] += 1;
  }
  d->_g159 = d->_GO || d->_g106 || d->_g97 || d->_g159_e1 || d->_g87 && !d->_cg92;
  d->_g114 = d->_g109 && !d->_cg110;
  if (d->_g114) {
    d->_taken_transitions[16] += 1;
  }
  d->_g111 = d->_g114 || d->_g168_e2 && !d->_cg116;
  d->_g92 = d->_pg160;
  if (d->_g92) {
    d->_region4_LRI_t += d->deltaT;
  }
  d->_g117 = d->_pg126;
  d->_g115 = d->_pg140;
  d->_g110 = !(d->_g117 || d->_g115);
  d->_g112 = d->_pg131;
  d->_cg134 = d->V_EVT;
  d->_g116 = d->_g112 && d->_cg134;
  if (d->_g116) {
    d->_Pacemaker_local__Atrig8 = 1;
  }
  d->_g137 = d->_g112 && !d->_cg134;
  d->_cg137 = d->_region4_LRI_t >= d->T_LRI;
  d->_g138 = d->_g137 && d->_cg137;
  if (d->_g138) {
    d->_Pacemaker_local__Atrig9 = 1;
  }
  d->_g130 = d->_g115 || d->_g117;
  d->_cg125 = d->_region4_LRI_t < d->T_LRI;
  d->_g124 = d->_g130 && d->_cg125;
  if (d->_g124) {
    d->sleepT = (d->sleepT < (d->T_LRI - d->_region4_LRI_t)) ? d->sleepT : (d->T_LRI - d->_region4_LRI_t);
  }
  d->_cg126 = d->_Pacemaker_local__Atrig8 || d->_Pacemaker_local__Atrig9;
  d->_g125 = d->_g130 && !d->_cg125;
  d->_cg128 = d->_Pacemaker_local__Atrig8 || d->_Pacemaker_local__Atrig9;
  d->_g127 = d->_g124 && d->_cg126 || d->_g125 && d->_cg128;
  d->_g134 = !d->_g112;
  d->_g138 = d->_g116 || d->_g138;
  d->_g135 = (d->_g110 || d->_g127) && (d->_g134 || d->_g138) && (d->_g127 || d->_g138);
  d->_cg139 = d->_Pacemaker_local__Atrig8;
  d->_g127 = d->_g135 && d->_cg139;
  if (d->_g127) {
    d->_region4_LRI_t = 0;
    d->_taken_transitions[19] += 1;
  }
  d->_g127_e1 = d->_pg120;
  d->_cg143 = d->V_EVT;
  d->_g136 = d->_g127_e1 && d->_cg143;
  if (d->_g136) {
    d->_taken_transitions[21] += 1;
  }
  d->_g136_e2 = d->_g127_e1 && !d->_cg143;
  d->_cg145 = d->uri_active == 0;
  d->_g143 = d->_g136_e2 && d->_cg145;
  if (d->_g143) {
    d->VP |= 1;
    d->_region4_LRI_t = 0;
    d->_taken_transitions[22] += 1;
  }
  d->_g146 = d->_GO || d->_g127 || d->_g136 || d->_g143;
  if (d->_g146) {
    d->_Pacemaker_local__Atrig8 = 0;
    d->_Pacemaker_local__Atrig9 = 0;
  }
  d->_cg121 = d->_region4_LRI_t < d->T_LRI;
  d->_g144 = d->_g146 && d->_cg121;
  if (d->_g144) {
    d->sleepT = (d->sleepT < (d->T_LRI - d->_region4_LRI_t)) ? d->sleepT : (d->T_LRI - d->_region4_LRI_t);
  }
  d->_cg122 = d->_Pacemaker_local__Atrig8 || d->_Pacemaker_local__Atrig9;
  d->_g140 = d->_g144 && !d->_cg122 || d->_g124 && !d->_cg126;
  d->_g122 = d->_g146 && !d->_cg121;
  d->_cg131 = d->_Pacemaker_local__Atrig8 || d->_Pacemaker_local__Atrig9;
  d->_g126 = d->_g125 && !d->_cg128 || d->_g122 && !d->_cg131;
  d->_g131 = d->_g146 || d->_g137 && !d->_cg137;
  d->_g128 = d->_g135 && !d->_cg139;
  if (d->_g128) {
    d->_taken_transitions[20] += 1;
  }
  d->_g120 = d->_g128 || d->_g136_e2 && !d->_cg145;
  d->_g137 = d->_GO || d->_g168 || d->_g107 && !d->_cg150;
  if (d->_g97_e1) {
    d->_region5_URI_t = 0;
    d->_taken_transitions[23] += 1;
  }
  d->_g139 = d->_g167 && d->_cg171;
  if (d->_g139) {
    d->_region5_URI_t = 0;
    d->_taken_transitions[24] += 1;
  }
  d->_g145 = d->_g97_e1 || d->_g139;
  if (d->_g145) {
    d->_Pacemaker_local__Atrig10 = 0;
    d->_Pacemaker_local__Atrig11 = 0;
  }
  d->_cg153 = d->_region5_URI_t < d->T_URI;
  d->_g141 = d->_g145 && d->_cg153;
  if (d->_g141) {
    d->sleepT = (d->sleepT < (d->T_URI - d->_region5_URI_t)) ? d->sleepT : (d->T_URI - d->_region5_URI_t);
  }
  d->_cg154 = d->_Pacemaker_local__Atrig10 || d->_Pacemaker_local__Atrig11;
  d->_g150 = d->_g141 && !d->_cg154 || d->_g162 && !d->_cg158;
  d->_g173 = d->_g145 && !d->_cg153;
  d->_cg163 = d->_Pacemaker_local__Atrig10 || d->_Pacemaker_local__Atrig11;
  d->_g171 = d->_g157 && !d->_cg160 || d->_g173 && !d->_cg163;
  d->_g172 = d->_g145 || d->_g169 && !d->_cg169;
  d->_g151 = d->_GO || d->_g181;
  d->_g154 = d->_GO || d->_g185;
  d->_g158 = d->_GO || d->_g63;
  d->_g163 = d->_GO || d->_g86;
  d->_g160 = d->_GO || d->_g92;
  d->_g152 = d->_GO || d->_g201;
  if (d->_g42_e1) {
    d->_reg_AP = d->AP;
  }
  if (d->_g13_e1) {
    d->_reg_VP = d->VP;
  }
}

void reset(TickData* d) {
  d->_GO = 1;
  d->_TERM = 0;
  d->T_AVI = AVI_VALUE;
  d->T_AEI = AEI_VALUE;
  d->T_PVAR = PVARP_VALUE;
  d->T_VRP = VRP_VALUE;
  d->T_LRI = LRI_VALUE;
  d->T_URI = URI_VALUE;
  d->_region0_VRefrac_t = 0;
  d->_region1_ARefrac_t = 0;
  d->_region2_AEI_t = 0;
  d->_region3_AVI_t = 0;
  d->_region4_LRI_t = 0;
  d->_region5_URI_t = 0;
  d->deltaT = 0.0;
  d->sleepT = 0.0;
  d->_reg_AP = 0;
  d->_reg_VP = 0;
  d->_taken_transitions[0] = 0;
  d->_taken_transitions[1] = 0;
  d->_taken_transitions[2] = 0;
  d->_taken_transitions[3] = 0;
  d->_taken_transitions[4] = 0;
  d->_taken_transitions[5] = 0;
  d->_taken_transitions[6] = 0;
  d->_taken_transitions[7] = 0;
  d->_taken_transitions[8] = 0;
  d->_taken_transitions[9] = 0;
  d->_taken_transitions[10] = 0;
  d->_taken_transitions[11] = 0;
  d->_taken_transitions[12] = 0;
  d->_taken_transitions[13] = 0;
  d->_taken_transitions[14] = 0;
  d->_taken_transitions[15] = 0;
  d->_taken_transitions[16] = 0;
  d->_taken_transitions[17] = 0;
  d->_taken_transitions[18] = 0;
  d->_taken_transitions[19] = 0;
  d->_taken_transitions[20] = 0;
  d->_taken_transitions[21] = 0;
  d->_taken_transitions[22] = 0;
  d->_taken_transitions[23] = 0;
  d->_taken_transitions[24] = 0;
  d->_taken_transitions[25] = 0;
  d->_pg8 = 0;
  d->_pg29 = 0;
  d->_pg17 = 0;
  d->_pg151 = 0;
  d->_pg13_e1 = 0;
  d->_pg27 = 0;
  d->_pg37 = 0;
  d->_pg34 = 0;
  d->_pg41 = 0;
  d->_pg154 = 0;
  d->_pg52 = 0;
  d->_pg42_e1 = 0;
  d->_pg62 = 0;
  d->_pg72 = 0;
  d->_pg158 = 0;
  d->_pg83_e2 = 0;
  d->_pg74 = 0;
  d->_pg77 = 0;
  d->_pg100 = 0;
  d->_pg163 = 0;
  d->_pg159 = 0;
  d->_pg98 = 0;
  d->_pg101 = 0;
  d->_pg111 = 0;
  d->_pg137 = 0;
  d->_pg171 = 0;
  d->_pg150 = 0;
  d->_pg172 = 0;
  d->_pg152 = 0;
  d->_pg160 = 0;
  d->_pg126 = 0;
  d->_pg140 = 0;
  d->_pg131 = 0;
  d->_pg120 = 0;
}

void tick(TickData* d) {
  logic(d);

  d->_pg8 = d->_g8;
  d->_pg29 = d->_g29;
  d->_pg17 = d->_g17;
  d->_pg151 = d->_g151;
  d->_pg13_e1 = d->_g13_e1;
  d->_pg27 = d->_g27;
  d->_pg37 = d->_g37;
  d->_pg34 = d->_g34;
  d->_pg41 = d->_g41;
  d->_pg154 = d->_g154;
  d->_pg52 = d->_g52;
  d->_pg42_e1 = d->_g42_e1;
  d->_pg62 = d->_g62;
  d->_pg72 = d->_g72;
  d->_pg158 = d->_g158;
  d->_pg83_e2 = d->_g83_e2;
  d->_pg74 = d->_g74;
  d->_pg77 = d->_g77;
  d->_pg100 = d->_g100;
  d->_pg163 = d->_g163;
  d->_pg159 = d->_g159;
  d->_pg98 = d->_g98;
  d->_pg101 = d->_g101;
  d->_pg111 = d->_g111;
  d->_pg137 = d->_g137;
  d->_pg171 = d->_g171;
  d->_pg150 = d->_g150;
  d->_pg172 = d->_g172;
  d->_pg152 = d->_g152;
  d->_pg160 = d->_g160;
  d->_pg126 = d->_g126;
  d->_pg140 = d->_g140;
  d->_pg131 = d->_g131;
  d->_pg120 = d->_g120;
  d->_GO = 0;
}
