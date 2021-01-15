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
#define CELL_CONNECT_SECONDS 90 // time to attempt to connect to cellular
#define CLOUD_CONNECT_SECONDS 60 // time to attempt to connect cloud
#define CLOUD_WAIT 30

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
  Time.zone(-7);

  // initGPS();
  // Engage cellular and cloud to get the time.now
  int connectionError = connectToCell(CELL_CONNECT_SECONDS);
  if (!connectionError)
  {
    connectionError = connectToCloud(CLOUD_CONNECT_SECONDS);
  }
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // delay(5000);
  // Serial.println("on");
  // digitalWrite(I2C_POWER,HIGH);
  // delay(5000);
  // GPS_TMP();
  // delay(5000);
  // Serial.println("off");
  // digitalWrite(I2C_POWER,LOW);
  // delay(10000);
  // SystemSleepConfiguration config;
  // config.mode(SystemSleepMode::ULTRA_LOW_POWER)
  //       .duration(20 * 1000);  // Takes msec
  // System.sleep(config);
  // Engage cellular and cloud to get the time.now
  int connectionError = connectToCell(CELL_CONNECT_SECONDS);
  if (!connectionError)
  {
    connectionError = connectToCloud(CLOUD_CONNECT_SECONDS);
    if (!connectionError)
    {
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
// Init GPS
// Initialize the GPS
// void initGPS()
// {
//     digitalWrite(I2C_POWER, HIGH);
//     delay(500);
//     Wire.begin();
//     if (myGPS.begin() == false) //Connect to the Ublox module using Wire port
//     {
//         Serial.printlnf("Ublox GPS not detected at default I2C address. Please check wiring.");
//     }
//     else
//     {
//         Serial.println("GPS detected.");
//     }
//     myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
//     myGPS.saveConfiguration(); //Save the settings to flash and BBR
//     Serial.printlnf("GPS = %f, %f. ", (float)myGPS.getLatitude() / 10000000, (float)myGPS.getLongitude() / 10000000);
//     delay(500);
//     digitalWrite(I2C_POWER, LOW);
//     delay(200);
// }


// int GPS_TMP()
// {
//     float lat = myGPS.getLatitude() / 10000000;
//     float lg = myGPS.getLongitude() / 10000000;
//     Serial.printlnf("GPS = %f, %f. ", lat, lg);
//     String results = String((float)myGPS.getLatitude() / 10000000)
//             + ", " + String((float)myGPS.getLongitude() / 10000000);
//     Particle.publish("GPS", results,PRIVATE);
//     delay(1000);
//     return 1;
// }

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
      return 1;
      break;
    }
    delay(1000);
  }
  Serial.printlnf("OK.");
  return 0;
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
      return 2;
      break;
    }
    delay(1000);
  }
  Serial.printlnf("OK.");
  return 0;
}