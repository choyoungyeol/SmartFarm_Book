#include <dht.h>                  //dht.zip 라이브러리를 설치해야 함
#define DHT22_PIN 7
dht DHT;

float temperature;
float humidity;

void setup()
{
  Serial.begin(9600);             //시리얼모니터 출력
  delay(300);
}

void loop()
{
  float chk = DHT.read22(DHT22_PIN);

  for (int i = 0; i < 100; i++) {            //평균온도
    temperature += DHT.temperature;
    delay(10);
  }
  temperature /= 10;
  for (int i = 0; i < 100; i++) {            //평균상대습도
    humidity += DHT.humidity;
    delay(10);
  }
  humidity /= 10;
  Serial.print(temperature, 1);              //평균온도값 출력
  Serial.println(" C");
  Serial.print(humidity, 1);                  //평균상대습도값 출력
  Serial.println(" %");
}
