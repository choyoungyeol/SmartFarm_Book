#include <dht.h>
#include <SoftwareSerial.h>
#define DHT22_PIN 7

SoftwareSerial BTSerial(2, 3);
dht DHT;

float temperature ;
float humidity ;
float After_temperature;       //온도의 오류값을 대비한 변수
float After_humidity;          //상대습도의 오류값을 대비한 변수

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

  if ((temperature <= -10) || (humidity > 100)) {     //오류값에 대한 검증
    temperature = After_temperature;                  //이전값을 현재값에 입력 
    humidity = After_humidity;
  }
  After_temperature = temperature;                    //현재값을 이전값에 입력
  readString = "";
  String data = "$" + String(temperature) + "/" + String(humidity) + "$";
  Serial.println(data);

  while (Serial.available()) {
  delay(3);
    char c = Serial.read();
    readString += c;
  }
  delay(1000);
}
