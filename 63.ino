#include <dht.h>                  //dht.zip 라이브러리를 설치해야 함
#define DHT22_PIN 7

dht DHT;

float temperature;
float humidity;
float After_temperature;       //온도의 오류값을 대비한 변수
float After_humidity;          //상대습도의 오류값을 대비한 변수

void setup()
{
  Serial.begin(9600);             //시리얼모니터 출력
  delay(300);
}

void loop()
{
  float chk = DHT.read22(DHT22_PIN); 

  temperature = DHT.temperature;         //온도
  humidity = DHT.humidity;                //상대습도

  if ((temperature <= -10) || (humidity > 100)) {     //오류값에 대한 검증
    temperature = After_temperature;                  //이전값을 현재값에 입력 
    humidity = After_humidity;
  }
  After_temperature = temperature;                    //현재값을 이전값에 입력
  After_humidity = humidity;

  
  Serial.print(temperature, 1);              //온도 현재값 출력
  Serial.println(" C");                       

  Serial.print(humidity, 1);                  //상대습도 현재값 출력
  Serial.println(" %");                       

  delay(1000);                             
}
