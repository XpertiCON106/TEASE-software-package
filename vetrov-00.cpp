#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS3231.h>

const int chipSelect = 4 ;

#define ONE_WIRE_BUS 8

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DS3231 rtc(SDA, SCL);


void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  SD.remove("test02.txt");
  sensors.begin();
  rtc.begin();
  rtc.setTime(0, 0, 0);
}

void loop() {
  // Send time
  Serial.println(rtc.getTimeStr());
  // Wait one second before repeating
  delay (1000);

  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);

  sensors.requestTemperatures();

  Serial.print("Temperature is: ");
  Serial.println(sensors.getTempCByIndex(0));
  Serial.print("The Voltage is: ");
  Serial.println(voltage);
  delay(1000);

  File dataFile = SD.open("test02.txt", FILE_WRITE);
  if (dataFile) {
    if (voltage > 0) {
      dataFile.print("Current voltage: ");
      dataFile.println(voltage);
      Serial.println("voltage data logged");
    } else {
      Serial.print("Voltage too low! ");
      Serial.println("Data not logged.");
    }

    dataFile.print("Current temperature: ");
    dataFile.println(sensors.getTempCByIndex(0));
    Serial.println("Temp logged in");
    dataFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}
