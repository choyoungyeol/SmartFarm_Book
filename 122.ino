#include <SD.h>   
#include <Wire.h>  
#include <RTClib.h> 
#include <SPI.h>
#include <dht.h>
#include <SoftwareSerial.h>
#define DHT22_PIN 7                  //온도 D7 연결
#define Temp_Cont 5                  //D5 온도제어
#define RH_Cont 6                    //D6 상대습도 제어

const int CS_PIN      = 10;           //D10
const int SD_POW_PIN  = 8;          //D8
const int RTC_POW_PIN = A3;        //A3
const int RTC_GND_PIN = A2;        //A2
const int IR_PIN      = 0;            //A0

RTC_DS1307 RTC;

String year, month, day, hour, minute, second, time, date;

int raw = 0;
int raw_prev = 0;
boolean active = false;
int update_time = 0;

File dataFile;
int n = 1;
char filename[] = "LOGGER00.CSV";           //저장 파일명

SoftwareSerial BTSerial(0, 1);
dht DHT;
float temperature;
float After_temperature;
float humidity;
float After_humidity;
int Temp_Value;
int RH_Value;
int VB_Temp_Cont = 0;
int VB_RH_Cont = 0;

String readString;

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(Temp_Cont, OUTPUT);
  delay(300);
  pinMode(RH_Cont, OUTPUT);
  delay(300);

  pinMode(CS_PIN,   OUTPUT);
  pinMode(SD_POW_PIN, OUTPUT);
  pinMode(RTC_POW_PIN, OUTPUT);
  pinMode(RTC_GND_PIN, OUTPUT);

  digitalWrite(SD_POW_PIN, HIGH);
  digitalWrite(RTC_POW_PIN, HIGH);
  digitalWrite(RTC_GND_PIN, LOW);

  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning())
  {
    Serial.println(F("RTC is NOT running!"));
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  if (!SD.begin(CS_PIN))
  {
    //Serial.println(F("Card Failure"));
    return;
  }
    //Serial.println(F("Card Ready"));

  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i / 10 + '0';
    filename[7] = i % 10 + '0';
    if (! SD.exists(filename)) {
      dataFile = SD.open(filename, FILE_WRITE);
      break; 
    }
  }
  if (! RTC.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop()
{
  int n = n + 1;
  DateTime datetime = RTC.now();
  float chk = DHT.read22(DHT22_PIN);

  temperature = DHT.temperature;
  humidity = DHT.humidity;

  if ((temperature <= -10) || (humidity > 100)) {
    temperature = After_temperature;
    humidity = After_humidity;
  }
  After_temperature = temperature;
  After_humidity = humidity;

  if (temperature >= 24) {
    digitalWrite(Temp_Cont, HIGH);      //온도 24℃ 이상에서 릴레이 작동 ON
    Temp_Value = 102;
    VB_Temp_Cont = 1;
  } else {
    digitalWrite(Temp_Cont, LOW);     //온도 24℃ 이하에서 릴레이 작동 OFF
    Temp_Value = 101;
    VB_Temp_Cont = 0;
  }

  if (humidity >= 40) {
    digitalWrite(RH_Cont, HIGH);        //상대습도 40% 이상에서 릴레이 작동 ON
    RH_Value = 104;
    VB_RH_Cont = 1;
  } else {
    digitalWrite(RH_Cont, LOW);       //상대습도 40% 이하에서 릴레이 작동 OFF
    RH_Value = 103;
    VB_RH_Cont = 0;
  }

  readString = "";
  String data = "$" + String(temperature) + "/" + String(humidity) + "/" + String(Temp_Value) + "/" + String(RH_Value) + "/" + String(VB_Temp_Cont) + "/" + String(VB_RH_Cont) + "$";
  Serial.println(data);

File dataFile = SD.open(filename, FILE_WRITE);           //SD 카드 저장
  if (dataFile) {
    dataFile.print(n);
    dataFile.print(",");
    dataFile.print(datetime.year(), DEC);
    dataFile.print(",");
    dataFile.print(datetime.month(), DEC);
    dataFile.print(",");
    dataFile.print(datetime.day(), DEC);
    dataFile.print(",");
    dataFile.print(datetime.hour(), DEC);
    dataFile.print(",");
    dataFile.print(datetime.minute(), DEC);
    dataFile.print(",");
    dataFile.print(temperature, 1);
    dataFile.print(",");
    dataFile.print(humidity, 1);
    dataFile.print(",");
    dataFile.print(VB_Temp_Cont);
    dataFile.print(",");
    dataFile.print(VB_RH_Cont);
    dataFile.print(",");
    dataFile.println("");
    dataFile.close();
    n = n + 1;
  }
  delay(5000);
}
