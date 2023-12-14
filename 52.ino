#include <dht.h>
#include <SoftwareSerial.h>
#define DHT22_PIN 7
#define Temp_Cont 8                   //온도 D8에서 제어
#define RH_Cont 9                     //상대습도 D9에서 제어

SoftwareSerial BTSerial(0, 1);
dht DHT;
float temperature ;
float humidity ;

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(Temp_Cont, OUTPUT);           //D8에서 출력
  delay(300);
  pinMode(RH_Cont, OUTPUT);              //D9에서 출력
  delay(300);
}

void loop()
{
  float chk = DHT.read22(DHT22_PIN);
  temperature = DHT.temperature;
  humidity = DHT.humidity;

  while (Serial.available()) {
    delay(3);
    char c = Serial.read();
  }

  Serial.print(temperature, 1);
  Serial.println(" C");
  Serial.print(humidity, 1);
  Serial.println(" %");

  if (temperature >= 24) {
    digitalWrite(Temp_Cont, HIGH);      //온도 24℃ 이상에서 릴레이 작동 ON
  } else {
    digitalWrite(Temp_Cont, LOW);     //온도 24℃ 이하에서 릴레이 작동 OFF 
  }

  if (humidity >= 40) {
    digitalWrite(RH_Cont, HIGH);        //상대습도 40% 이상에서 릴레이 작동 ON
  } else {
    digitalWrite(RH_Cont, LOW);       //상대습도 40% 이하에서 릴레이 작동 OFF
  }
  delay(5000);
}
