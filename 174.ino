#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
#define Left_Open_Window 4
#define Left_Close_Window 5
#define Right_Open_Window 6
#define Right_Close_Window 7

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

uint8_t sensor1[8] = { 0x28, 0xFF, 0x6E, 0x73, 0x71, 0x17, 0x04, 0x7C };
uint8_t sensor2[8] = { 0x28, 0xFF, 0x2B, 0xFB, 0x71, 0x17, 0x04, 0x82 };

float Dry_Temp;
float Wet_Temp;

int Set_Temp = 25;
int Limit_Temp = 3;

void setup(void)
{
  pinMode(Left_Open_Window, OUTPUT);
  pinMode(Left_Close_Window, OUTPUT);
  pinMode(Right_Open_Window, OUTPUT);
  pinMode(Right_Close_Window, OUTPUT);
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{
  float Patm = 101.325;   //기압 kPa
  sensors.requestTemperatures();
  Wet_Temp = sensors.getTempC(sensor1);
  Dry_Temp = sensors.getTempC(sensor2);
  float Psdt = 0.61078 * exp((17.27 * Dry_Temp) / (237.3 + Dry_Temp)); //kPa
  float Pswt = 0.61078 * exp((17.27 * Wet_Temp) / (237.3 + Wet_Temp)); //kPa
  float Pv = Pswt - Patm * 0.000662 * (Dry_Temp - Wet_Temp);
  float Humidity = 100 * Pv / Psdt;
  float VPD = Psdt * (1 - Humidity / 100);
  float Dew_Temp = (Dry_Temp - (100 - Humidity) / 5);
  float AH = 2.16679 * Pv / (273.15 + Dry_Temp);

  Serial.print("Dry Temp : ");
  Serial.println(Dry_Temp);
  Serial.print("Wet Temp: ");
  Serial.println(Wet_Temp);
  Serial.print("Dew Temp : ");
  Serial.println(Dew_Temp);
  Serial.print("Humidity : ");
  Serial.println(Humidity);
  Serial.print("AH : ");
  Serial.println(AH);
  Serial.print("SVP : ");
  Serial.println(Psdt);
  Serial.print("Pv : ");
  Serial.println(Pv);
  Serial.print("VPD : ");
  Serial.println(VPD);
  delay(5000);

  if (VPD >= 0.5 && VPD <= 1.2) {
    digitalWrite(Left_Open_Window, LOW);
    digitalWrite(Left_Close_Window, LOW);
    digitalWrite(Right_Open_Window, LOW);
    digitalWrite(Right_Close_Window, LOW);
  }

  if (VPD < 0.5 ) {
    if (Dry_Temp < (Set_Temp - Limit_Temp)) {
      digitalWrite(Left_Open_Window, LOW);
      digitalWrite(Left_Close_Window, HIGH);
      delay(2000);
      digitalWrite(Right_Open_Window, LOW);
      digitalWrite(Right_Close_Window, HIGH);
      delay(2000);
    }

    if (Dry_Temp > (Set_Temp + Limit_Temp)) {
      digitalWrite(Left_Open_Window, HIGH);
      delay(2000);
      digitalWrite(Left_Close_Window, LOW);
      digitalWrite(Right_Open_Window, HIGH);
      delay(2000);
      digitalWrite(Right_Close_Window, LOW);
    }
  }

  if (VPD > 1.2) {
    if (Dry_Temp < (Set_Temp - Limit_Temp)) {
      digitalWrite(Left_Open_Window, LOW);
      digitalWrite(Left_Close_Window, HIGH);
      delay(2000);
      digitalWrite(Right_Open_Window, LOW);
      digitalWrite(Right_Close_Window, HIGH);
      delay(2000);
    }

    if (Dry_Temp > (Set_Temp + Limit_Temp)) {
      digitalWrite(Left_Open_Window, HIGH);
      delay(2000);
      digitalWrite(Left_Close_Window, LOW);
      digitalWrite(Right_Open_Window, HIGH);
      delay(2000);
      digitalWrite(Right_Close_Window, LOW);
    }
  }
}
