/*
 * timing.h
 *
 *  Created on: 15/10/2025
 *      Author: ylee903
 */


// timing.h � all times in milliseconds
#define AVI_VALUE 300 //The maximum time between an atrial event and its subsequent ventricular event; Atrioventricular Interval 
#define AEI_VALUE 800 //The maximum time between a ventricular event and its subsequent atrial event. ;Atrial Escape Interval 
#define PVARP_VALUE 50 // The time after a ventricular event where any atrial events are ignored as Atrial Refractory (AR) signals.; Post-Ventricular Atrial Refractory Period 
#define VRP_VALUE 150 //The time after a ventricular event where any other ventricular events are ignored as Ventricular Refractory (VR) signals. ; Ventricular Refractory Period 
#define LRI_VALUE 950 // The slowest rate at which the heart is allowed to operate. This is measured as the time between ventricular events.; Lower Rate Interval 
#define URI_VALUE 900 // The fastest rate at which the pacemaker will ever pace the heart at. This is measured as the time between ventricular events. Upper Rate Interval 

