#include <dht.h>                  //dht.zip 라이브러리를 설치해야 함
#define DHT22_PIN 7
#define num 20                 //20개 측정값

dht DHT;

float temp[num];                //배열 작성
float humi[num];                //배열 작성
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

  for (int i = 0; i < num - 1; i++) {    
    temp[i] = temp[i + 1];
  }
  temp[num - 1] = DHT.temperature;

  for (int i = 0; i < num; i++) {           
    temperature += temp[i];
  }
  temperature /= num;                         //평균온도
  delay(10);
  
  for (int i = 0; i < num - 1; i++) {
    humi[i] = humi[i + 1];
  }
  humi[num - 1] = DHT.humidity;

  for (int i = 0; i < num; i++) {            
    humidity += humi[i];
  }
  humidity /= num;                           //평균상대습도
  delay(10);
  Serial.print(temperature, 1);                //평균온도값 출력
  Serial.println(" C");
  Serial.print(humidity, 1);                   //평균상대습도값 출력
  Serial.println(" %");
  delay(500);
}
