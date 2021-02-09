/** -------------------------------------------------------------------------- 
 * File: ClubLightingController.h
 * Created Date: 11-18-2020
 * Author: Thomas Ose
 * ---------------------------------------------------------------------------
 * Last Modified: Monday, 8th February 2021 6:22:17 pm
 * Modified By: Thomas Ose (tmo@osemicro.com>)
 * ---------------------------------------------------------------------------
 * Copyright (c) 2016 - 2020 OMS(re)Models
 * 
 *  This source file is subject of the GNU general public license 2,
 *  that is available at the world-wide-web at
 *  http://www.gnu.org/licenses/gpl.txt
 * 
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  copy of this software and associated documentation files.  The above
 *  copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 * ---------------------------------------------------------------------------
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	----------------------------------------------------------
 * 11-22-2020	tmo	Removed unused code and change sampling times
 * 11-18-2020	tmo	Initial file creation
 * --------------------------------------------------------------------------- */

#ifndef _CLUBLIGHTINGCONTROLLER_h
#define _CLUBLIGHTINGCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "TaskScheduler.h"

// Define variables
#define VERSION 1.01
//#define DEBUG

#define RELAY_1 4
#define RELAY_2 5
#define RELAY_3 6
#define RELAY_4 7

#define LIGHT_SENSOR_THRESHHOLD 1015
#define LIGHT_SENSOR A1
#define DELAY_POT_1 A2
#define DELAY_POT_2 A3

#define HEART_BEAT 13
#define SECONDS 1000 // 1000 miliseconds
#ifdef DEBUG
#define MINUTES 1000       // 1 second test time
#define DEFAULT_DELAY 6000 // 6 second test time
#else
#define MINUTES 60000        // 60 Seconds
#define DEFAULT_DELAY 600000 // 10 minutes
#endif

#define ON LOW
#define OFF HIGH

// variables to read and set the delay times form the potentiometers
int DelayRead1 = 0;
int DelayRead2 = 0;

int LastSensorRead = 0;
bool LightRequire = true;
bool LastLightRequire = true;
bool LastLightStatus = true;
unsigned long LightRequireStartTime = 0;

// variable used to controll the delay time
unsigned long DelayTime1 = DEFAULT_DELAY;
unsigned long DelayTime2 = DEFAULT_DELAY;
unsigned long DelayStart1 = 0;
unsigned long DelayStart2 = 0;

// task scheduler variables
Scheduler runner; // the main runner task

void CheckSensor_Callback();
void CheckRelays_CallBack();
void CheckDelayPot_CallBack();
void CheckHeartBeat_CallBack();

Task CheckSensor(2500, TASK_FOREVER, &CheckSensor_Callback);
Task CheckRelays(2000, TASK_FOREVER, &CheckRelays_CallBack);
Task CheckDelayPot(5000, TASK_FOREVER, &CheckDelayPot_CallBack);
Task CheckHeartBeat(1000, TASK_FOREVER, &CheckHeartBeat_CallBack);

#endif