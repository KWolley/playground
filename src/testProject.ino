/*
* Project testProject
* Description: Ultra Low Power Sleep Testing
* Author: Kara Wolley
* Date: 11/30/2020
*/
// #include "SparkFun_Ublox_Arduino_Library.h"
#include "Particle.h"
#include "stdint.h"

// #define I2C_POWER           D17     // Turn off the I2C when sleeping as GPS guzzles current 

// SFE_UBLOX_GPS myGPS;        // The GPS object for ZOE-M8Q
// SYSTEM_MODE(MANUAL);  

// #define CELL_CONNECT_SECONDS 90 // time to attempt to connect to cellular
// #define CLOUD_CONNECT_SECONDS 60 // time to attempt to connect cloud
// #define CLOUD_WAIT 30

// setup() runs once, when the device is first turned on.
void setup() {
  Serial.begin(9600);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  
  String s = "02/01/21,02/10/21,60";
  Serial.printlnf("size of string is %d",sizeof(s));
  
  String s1 = s.substring(0,8);
  String s2 = s.substring(9,17);
  String s3 = s.substring(18,20);

  Serial.println(s1);
  Serial.println(s2);
  Serial.println(s3);

  // convert string to timestamp
  // note this will revert to GMT time - need to change to time zone of device
  struct tm tm1;
  struct tm tm2;
  time_t t1, t2;

  size_t a =  sizeof(struct tm);
  memset(&tm1, 0, a);
  memset(&tm2,0, a);

  strptime(s1,"%m/%d/%y",&tm1);
  strptime(s2,"%m/%d/%y",&tm2);
  
  t1 = mktime(&tm1);  //sec
  t2 = mktime(&tm2); //sec

  uint32_t startDateUnix = (uint32_t) t1;
  uint32_t endDateUnix = (uint32_t) t2;

  char*ptr;
  long mP = strtol(s3,&ptr,10);
  Serial.printlnf("measurement period = %ld",mP);

  int epoch = endDateUnix - startDateUnix; // calc diff between start and end date (seconds)
  int timeFromNow =  Time.now() - endDateUnix; // calc time now to end date (seconds)
  int numRecordsSkip = timeFromNow / 60 / mP; // calc number of records to jump sec -> min / measurement period
  int numRecordsPublish = epoch / 60 / mP; // calc total number of records to publish sec -> min / measurement period
  Serial.println("startDate: "+Time.timeStr(startDateUnix));
  Serial.println("endDate: "+Time.timeStr(endDateUnix));
  Serial.println("time now: " + Time.format(Time.now()));
  Serial.printlnf("time from end to now: %d",timeFromNow);
  Serial.printlnf("startDateUnix: %ld, endDateUnix: %ld, epoch: %d",startDateUnix,endDateUnix,epoch);
  Serial.printlnf("number of records to skip: %d, number of records to publish: %d",numRecordsSkip,numRecordsPublish);
  

  // find list one location
  // set start address to location - number of records to skip
  // publish data
  
  delay(60000);
  
  
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