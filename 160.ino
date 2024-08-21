#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

uint8_t sensor1[8] = { 0x28, 0xFF, 0x6E, 0x73, 0x71, 0x17, 0x04, 0x7C };
uint8_t sensor2[8] = { 0x28, 0xFF, 0x2B, 0xFB, 0x71, 0x17, 0x04, 0x82 };

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{
  sensors.requestTemperatures();
  
  Serial.print("Sensor 1: ");
  printTemperature(sensor1);
  
  Serial.print("Sensor 2: ");
  printTemperature(sensor2);
  
  Serial.println();
  delay(1000);
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.println(tempC);
}
