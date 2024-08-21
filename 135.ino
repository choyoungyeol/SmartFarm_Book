#include<LiquidCrystal_I2C.h>
#include <SimpleDHT.h>
#include<SoftwareSerial.h>
#include <LoRa.h>
#define Sensor A1
#define pinDHT11 3
#define Pump_On 22
#define Pump_Off 24
#define Window_On 26
#define Window_Off 28
#define Fan_On 30
#define Fan_Off 32

LiquidCrystal_I2C lcd(0x27, 16, 2);
SimpleDHT11 dht11(pinDHT11);

int Target_Water = 30;
int Target_Temp = 25;
int Diff_Water = 5;
int Diff_Temp = 3;
int Pump_value;
int Window_value;
int Fan_value;
int Auto = 1;
String readString;

void setup() {
  pinMode(Pump_On, OUTPUT);
  pinMode(Pump_Off, OUTPUT);
  pinMode(Window_On, OUTPUT);
  pinMode(Window_Off, OUTPUT);
  pinMode(Fan_On, OUTPUT);
  pinMode(Fan_Off, OUTPUT);

  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(868E6)) { // or 915E6, the MHz speed of yout module
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("JEJU Nat'l Univ.");
  lcd.setCursor(0, 1);
  lcd.print("Hello Arduino !");
  delay(5000);
}

void loop() {
  int Water = analogRead(Sensor);
  int Soil_Water = map(Water, 588, 313, 0, 100);

  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Water = ");
  lcd.print(Soil_Water);
  lcd.print(" %");
  lcd.setCursor(0, 1);
  lcd.print("T=");
  lcd.print(temperature);
  lcd.print(" oC, RH=");
  lcd.print(humidity);
  lcd.print(" %");
  delay(5000);

  readString = "";
  if (Auto == 1) {
    if (Soil_Water < (Target_Water - Diff_Water)) {
      digitalWrite(Pump_On, HIGH);
      digitalWrite(Pump_Off, LOW);
      Pump_value = 1;
    }

    if ((Soil_Water >= (Target_Water - Diff_Water)) && (Soil_Water < (Target_Water + Diff_Water))) {
      digitalWrite(Pump_On, LOW);
      digitalWrite(Pump_Off, HIGH);
      Pump_value = 0;
    }

    if (Soil_Water >= (Target_Water + Diff_Water)) {
      digitalWrite(Pump_On, LOW);
      digitalWrite(Pump_Off, HIGH);
      Pump_value = 0;
    }

    if (temperature < (Target_Temp - Diff_Temp) ) {
      digitalWrite(Window_On, HIGH);
      digitalWrite(Window_Off, LOW);
      digitalWrite(Fan_On, HIGH);
      digitalWrite(Fan_Off, LOW);
      Window_value = 1;
      Fan_value = 1;
    }

    if ((temperature >= (Target_Temp - Diff_Temp)) && (temperature < (Target_Temp + Diff_Temp))) {
      digitalWrite(Window_On, LOW);
      digitalWrite(Window_Off, HIGH);
      digitalWrite(Fan_On, LOW);
      digitalWrite(Fan_Off, HIGH);
      Window_value = 0;
      Fan_value = 0;
    }

    if (temperature >= (Target_Temp + Diff_Temp)) {
      digitalWrite(Window_On, LOW);
      digitalWrite(Window_Off, HIGH);
      digitalWrite(Fan_On, LOW);
      digitalWrite(Fan_Off, HIGH);
      Window_value = 0;
      Fan_value = 0;
    }
  }

  while (Serial.available()) {
    delay(3);
    char c = Serial.read();
    readString += c;
  }

  if (readString.length() > 0) {
    if (readString == "x") {
      Auto = 1;
    }
    if (readString == "y") {
      Auto = 0;
    }
    if (readString == "a") {
      digitalWrite(Pump_On, HIGH);
      digitalWrite(Pump_Off, LOW);
      Pump_value = 1;

    }

    if (readString == "b") {
      digitalWrite(Pump_On, LOW);
      digitalWrite(Pump_Off, HIGH);
      Pump_value = 0;
    }
    if (readString == "c") {
      digitalWrite(Window_On, HIGH);
      digitalWrite(Window_Off, LOW);
      Window_value = 1;
    }
    if (readString == "d") {
      digitalWrite(Window_On, LOW);
      digitalWrite(Window_Off, HIGH);
      Window_value = 0;
    }

    if (readString == "e") {
      digitalWrite(Fan_On, HIGH);
      digitalWrite(Fan_Off, LOW);
      Fan_value = 1;
    }
    if (readString == "f") {
      digitalWrite(Fan_On, LOW);
      digitalWrite(Fan_Off, HIGH);
      Fan_value = 0;
    }
  }

  Auto = Auto;
  String data = String(Water) + "," + String(Soil_Water) + "," + int(temperature) + "," + String(humidity) + "," + String(Pump_value) + "," + String(Window_value) + "," + String(Fan_value) + "," + String(Auto)+ ",";
  Serial.println(data);

  LoRa.beginPacket();
  LoRa.print(data);
  LoRa.endPacket();
  delay(50);
}
