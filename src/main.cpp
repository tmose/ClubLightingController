/** -------------------------------------------------------------------------- 
 * File: main.cpp
 * Created Date: 11-18-2020
 * Author: Thomas Ose
 * ---------------------------------------------------------------------------
 * Last Modified: Saturday, 21st November 2020 4:07:34 pm
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
 * 11-21-2020	tmo	Started adding more processing code
 * 11-18-2020	tmo	Created original code
 * --------------------------------------------------------------------------- */

#include "ClubLightingController.h"

/**
 * @brief  CallBack function for the sensor check
 * @note   Read the photo cell and sees if the light is required.  Sets NEED_LIGHT appropriately.
 *          If the value changes then also resets the relay counters.
 * @retval None
 */
void CheckSensor_Callback()
{
    int value = analogRead(LIGHT_SENSOR);
    delay(10);
    Serial.println(value);
    if (LastSensorRead != value)
    {
        LastSensorRead = value;
        LightRequire = (LastSensorRead > LIGHT_SENSOR_THRESHHOLD);
        Serial.println(LightRequire);
        Serial.println(LastLightStatus);
        if (LightRequire != LastLightStatus)
        {
            if (LightRequire && !LastLightRequire)
            {
                LastLightRequire = true;
                Serial.println("Turn on lights");
            }
            if (!LightRequire && LastLightRequire)
            {
                LastLightRequire = false;
                Serial.println("Light Not Required");
            }
        }
        LastLightStatus = LightRequire;
    }
}

/**
 * @brief  Control all the relay outputs
 * @note   LightRequired controll the action of the relays.  
 *              Relay 1 is always on
 *              Relay 2 is on but shuts of after the time specified in DelayTime* when LightRequired is true
 *              Relay 3 stays on when LightRequired is true
 *              Relay 4 turn on with LightRequired but turns of after time period specified in DelayTime2
 * @retval None
 */
void CheckRelays_CallBack()
{
    if (LightRequire && LightRequireStartTime == 0)
    {
        LightRequireStartTime = millis();
        digitalWrite(RELAY_3, ON);
        digitalWrite(RELAY_4, ON);
    }
    if (!LightRequire && LightRequireStartTime != 0)
    {
        LightRequireStartTime = 0;
        digitalWrite(RELAY_2, ON);
        digitalWrite(RELAY_3, OFF);
        digitalWrite(RELAY_4, OFF);
    }
    unsigned long now = millis();
    if (LightRequire)
    {
        if (now - LightRequireStartTime >= DelayTime2)
            digitalWrite(RELAY_2, OFF);
        if (now - LightRequireStartTime >= DelayTime1)
            digitalWrite(RELAY_4, OFF);
    }
}

/**
 * @brief  Check Delay Potentiometer activity
 * @note   Read the Pot value and if changed calculate the setting by dividing the value by 100 getting 10 steps
 *          Multiple the steps by 10 minutes and add it to the default delay time.
 * @retval None
 */
void CheckDelayPot_CallBack()
{
    int delayRead1 = analogRead(DELAY_POT_1);
    delay(10);
    int delayRead2 = analogRead(DELAY_POT_2);
    delay(10);
    if (delayRead1 != DelayRead1)
    {
        DelayRead1 = delayRead1;
        DelayTime1 = DEFAULT_DELAY + ((DelayRead1 / 100) * (MINUTES * 10));
        Serial.print("Delay 1 Time Changed: ");
        Serial.println(DelayTime1);
    }
    if (delayRead2 != DelayRead2)
    {
        DelayRead2 = delayRead2;
        DelayTime2 = DEFAULT_DELAY + ((DelayRead2 / 100) * (MINUTES * 10));
        Serial.print("Delay 2 Time Changed: ");
        Serial.println(DelayTime2);
    }
}

/**
 * @brief  Heart beat routine
 * @note   Alternates the Arduino LED to indicate that the processor is working.
 * @retval None
 */
void CheckHeartBeat_CallBack()
{
    bool PinOn = digitalRead(HEART_BEAT);
    digitalWrite(HEART_BEAT, PinOn ? LOW : HIGH);
}

/**
 * @brief  Setup of default functions and values for the application
 * @note   
 * @retval None
 */
void setup()
{
#ifdef DEBUG // only write to the serial port if we are in debug mode
    Serial.begin(9600);
#endif

    pinMode(RELAY_1, OUTPUT);
    pinMode(RELAY_2, OUTPUT);
    pinMode(RELAY_3, OUTPUT);
    pinMode(RELAY_4, OUTPUT);

    pinMode(LIGHT_SENSOR, INPUT);
    pinMode(DELAY_POT_1, INPUT);
    pinMode(DELAY_POT_2, INPUT);
    pinMode(HEART_BEAT, OUTPUT);

    digitalWrite(RELAY_1, ON);
    digitalWrite(RELAY_2, ON);
    digitalWrite(RELAY_3, OFF);
    digitalWrite(RELAY_4, OFF);

    runner.init();

    runner.addTask(CheckSensor);
    CheckSensor.enable();
    Serial.println("Photo Sensor task added");

    runner.addTask(CheckRelays);
    CheckRelays.enable();
    Serial.println("Relay task added");

    runner.addTask(CheckHeartBeat);
    CheckHeartBeat.enable();
    Serial.println("Heartbeat task added");
}

/**
 * @brief  Main program loop
 * @note   
 * @retval None
 */
void loop()
{
    runner.execute();
}
