#include <dht.h>
#include <SoftwareSerial.h>               //SoftwareSerial.zip 라이브러리를 설치
#define DHT22_PIN 7                     //D7 사용

SoftwareSerial BTSerial(0, 1);             //D0와 D1 사용
dht DHT;

float temperature ;
float humidity ;

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

  while (Serial.available()) {                //문자 통신
    delay(3);
    char c = Serial.read();
  }

  Serial.print(temperature, 1);
  Serial.println(" C");
  Serial.print(humidity, 1);
  Serial.println(" %");
  delay(1000);
}
