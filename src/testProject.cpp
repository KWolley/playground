/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/karaw/Documents/aquarealtime/resdev/software/testProject/src/testProject.ino"
/*
 * Project testProject
 * Description: Ultra Low Power Sleep Testing
 * Author: Kara Wolley
 * Date: 11/27/2020
 */
  void setup();
void loop();
#line 7 "c:/Users/karaw/Documents/aquarealtime/resdev/software/testProject/src/testProject.ino"
uint32_t sleepTime;
  int ctr;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
  Time.zone(-7);
  sleepTime = 8 * 3600 * 1000; // 8 hours
  ctr = 0;
  for (uint32_t ms = millis(); millis() - ms < 10000; Particle.process());
  String dat = "Loop #: " + (String) ctr + " Time: " + Time.format(Time.now(), "%Y-%m-%d %H:%M:%S") + " BattSOC: " + (String) System.batteryCharge();
  Serial.println("======= SET UP ===========");
  Serial.println(dat);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  ctr++;
  // The core of your code will likely live here.
  String dat = "Loop #: " + (String) ctr + " Time: " + Time.format(Time.now(), "%Y-%m-%d %H:%M:%S") + " BattSOC: " + (String) System.batteryCharge();
  Serial.println(dat);
  // Serial.printlnf("Loop #: %d, Time: %Y-%m-%d %H:%M:%S, BattSOC: %d",ctr,Time.format(Time.now(), "%Y-%m-%d %H:%M:%S"), System.batteryCharge());
  SystemSleepConfiguration config;
  config.mode(SystemSleepMode::ULTRA_LOW_POWER)
            .duration(sleepTime); 
  for (uint32_t ms = millis(); millis() - ms < 10000; Particle.process());
  SystemSleepResult result = System.sleep(config);

}