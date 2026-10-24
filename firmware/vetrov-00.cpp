/*
   TEASE - Balloon Payload Proejct
   Contributor 1
   Immamul Morsilin
*/

/* importing libraries */
#include <SPI.h>
#include <SD.h>
 #include <OneWire.h>
#include <DallasTemperature.h>
#include <DS3231.h>

const int chipSelect = 4 ;

/* Temperature sensor setup */
#define ONE_WIRE_BUS 8
#define SECOND_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
OneWire secondWire(SECOND_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DallasTemperature sensors2(&secondWire);

DS3231 rtc(SDA, SCL);

void setup() {
  Serial.begin(9600);

  /* Checking if SD card works */
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");

  SD.remove("test02.txt");//removing old file

  /*starting temperature sensors */
  sensors.begin();
  sensors2.begin();

  /*Starting time clock */
  rtc.begin();
  rtc.setTime(0, 0, 0);
}

void loop() {

  File dataFile = SD.open("test02.txt", FILE_WRITE);

  // Serial printing time
  Serial.print("Time: ");
  Serial.println(rtc.getTimeStr());
  if (rtc.getTimeStr() <1
  
  )
  {
    dataFile.println("Start of data log");
  }

  if (rtc.getTimeStr() > 0)
  {
    // Data printing time
    dataFile.print("Time: ");
    dataFile.println(rtc.getTimeStr());
    Serial.println("Time logged in");

    int sensorValue = analogRead(A0);
    float voltage = sensorValue * (5.0 / 1023.0);

    /*Getting temperature data from two sensor */
    sensors.requestTemperatures();
    sensors2.requestTemperatures();

    /*printing temp data to serial */
    Serial.print("Outside temperature: ");
    Serial.println(sensors.getTempCByIndex(0));
    Serial.print("Inside temperature: ");
    Serial.println(sensors2.getTempCByIndex(0));

    /* printing temp data to SD*/
    dataFile.print("Outside temperature: ");
    dataFile.println(sensors.getTempCByIndex(0));
    dataFile.print("Inside temperature: ");
    dataFile.println(sensors2.getTempCByIndex(0));
    Serial.println("Temperature data logged in");

    /*printing voltage data */
    Serial.print("The Voltage is: ");
    Serial.println(voltage);

    if (dataFile) {
      if (voltage > 0) {
        Serial.print("Current voltage: ");
        Serial.println(voltage);
        dataFile.print("Current voltage: ");
        dataFile.println(voltage);

        Serial.println("voltage data logged");
        Serial.println("-------------------------------------------");
        dataFile.println("-----------------------------------------");
      } else {
        Serial.print("Voltage too low, ");
        Serial.println(" Data not logged!");
        Serial.println("-------------------------------------------");
        dataFile.print("Current voltage: ");
        dataFile.println(voltage);
        dataFile.println("------------------------------------------");
      }
      dataFile.close();
    } else {
      Serial.println("error opening test.txt");
    }
  }
  dataFile.println("-----------------------------------------");
}
