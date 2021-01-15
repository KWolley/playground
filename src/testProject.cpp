/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/karaw/Documents/aquarealtime/resdev/software/testProject/src/testProject.ino"
/*
* Project testProject
* Description: Ultra Low Power Sleep Testing
* Author: Kara Wolley
* Date: 11/30/2020
*/
// #include "SparkFun_Ublox_Arduino_Library.h"

// #define I2C_POWER           D17     // Turn off the I2C when sleeping as GPS guzzles current 

// SFE_UBLOX_GPS myGPS;        // The GPS object for ZOE-M8Q
void setup();
void loop();
int ULPSleep(int seconds);
int connectToCell(int timeoutSec);
int connectToCloud(int timeoutSec);
#line 12 "c:/Users/karaw/Documents/aquarealtime/resdev/software/testProject/src/testProject.ino"
SYSTEM_MODE(MANUAL);  

#define CELL_CONNECT_SECONDS 90 // time to attempt to connect to cellular
#define CLOUD_CONNECT_SECONDS 60 // time to attempt to connect cloud
#define CLOUD_WAIT 30

// setup() runs once, when the device is first turned on.
void setup() {
  Serial.begin(9600);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  delay(1000);
  int cellConnectTime = connectToCell(CELL_CONNECT_SECONDS);
  if (cellConnectTime > 0)
  {
    int cloudConnectTime = connectToCloud(CLOUD_CONNECT_SECONDS);
    if (cloudConnectTime > 0)
    {
      String outDat = "cell: " + (String) cellConnectTime + ",  cloud: " + (String) cloudConnectTime;
      Particle.publish("connectTime",outDat,PRIVATE);
      delay(1000);
      Particle.disconnect();
      Cellular.disconnect();
      Cellular.off();
    }
  }

  ULPSleep(60);
}

// Sleep ULP way
int ULPSleep(int seconds)
{
  if (seconds <= 0)
  {
    seconds = 10;
    Serial.printlnf("Zero sleep time, adjusting to 10 seconds sleep time.");
  }
  Serial.printlnf("Going to sleep for %d minutes %d seconds.", seconds / 60, seconds % 60);
  SystemSleepConfiguration config;
  config.mode(SystemSleepMode::ULTRA_LOW_POWER)
        .duration(seconds * 1000);  // Takes msec
  System.sleep(config);
  return 1;
}

// Connect to cellular
// Returns 1 if fail, 0 if success. Pass it seconds
int connectToCell(int timeoutSec)
{
  int secondCounter = 0;
  Serial.printf("Connecting to cellular ... ");
  Cellular.on();
  Cellular.connect();
  while (Cellular.ready() == false)
  {
    secondCounter ++;
    if (secondCounter >= timeoutSec)
    {
      Serial.printlnf("Couldn't connect to cellular.");
      Cellular.off();
      return 0;
      break;
    }
    delay(1000);
  }
  Serial.printlnf("OK.");
  return secondCounter;
}

//
int connectToCloud(int timeoutSec)
{
  Serial.printf("Connecting to cloud ... ");
  int secondCounter = 0;
  Particle.connect();
  while (Particle.connected() == false)
  {
    secondCounter ++;
    Particle.process();
    if (secondCounter >= timeoutSec)
    {
      Serial.printlnf("Couldn't connect to Cloud.");
      Cellular.disconnect();
      Cellular.off();
      return 0;
      break;
    }
    delay(1000);
  }
  Serial.printlnf("OK.");
  return secondCounter;
}