#include <dht.h>
#include <SoftwareSerial.h>
#define DHT22_PIN 7

SoftwareSerial BTSerial(0, 1);
dht DHT;

float temperature ;
float humidity ;
String readString;

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  delay(300);
}

void loop()
{
  float chk = DHT.read22(DHT22_PIN);

  temperature = DHT.temperature;
  humidity = DHT.humidity;

  readString = "";
  String data = String(temperature) + "," + String(humidity) + ",";
  Serial.println(data);

  while (Serial.available()) {
  delay(3);
    char c = Serial.read();
    readString += c;
  }
  delay(1000);
}
